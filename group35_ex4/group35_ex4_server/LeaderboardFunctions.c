// Authors - Tomer Segal 207133646, Nadav Nave 209365725
// Exercise 4 in the Course Introduction to system programming
// This is the functions of the leaderboard

#include "LeaderboardFunctions.h"


void RemovePlayerFromList(leaderboard_player **before_player, leaderboard_player **player, leaderboard_player **first)
{
	//the player is first in the list
	if (*before_player == NULL)
	{
		*first = (*first)->next;
	}
	//there is a player in the list
	else if (*player != NULL)
	{
		(*before_player)->next = (*player)->next;
		(*player)->next = NULL;
	}
}


int SearchInList(leaderboard_player **current_player, char *username, int gamestat, leaderboard_player **first_p)
{
	leaderboard_player *search_p = *first_p;
	leaderboard_player *search_p_before = NULL;
	int found_flag = 0, ret = 0, err = 0;

	//search the list for the username
	while (search_p != NULL)
	{
		if (STRINGS_ARE_EQUAL(username, search_p->username))
		{
			//if found remove it from the list
			RemovePlayerFromList(&search_p_before, &search_p, first_p);
			found_flag = 1;
			break;
		}
		search_p_before = search_p;
		search_p = search_p->next;
	}
	//if found update the current_player to the player with the same user name
	if (found_flag == 1)
		*current_player = search_p;
	//if player wasnt found allocate memory for the player struct
	else
	{
		(*current_player) = (leaderboard_player*)malloc(sizeof(leaderboard_player));
		if ((*current_player) == NULL)
		{
			printf("Error: allocating memory for leaderboard struct\n");
			ret = ERROR_CODE;
			goto return_ret;
		}
		err = sprintf_s((*current_player)->username, MAX_USERNAME + 1, "%s", username);
		if (err == 0 || err == EOF)
		{
			printf("Error: can't use sprintf for the struct");
			ret = ERROR_CODE;
			goto main_cleanup;
		}
		(*current_player)->lost = 0;
		(*current_player)->won = 0;
		(*current_player)->ratio = 0;
		(*current_player)->next = NULL;
	}
	//update his won/lost stats according to the game result
	if (gamestat == PLAYER_WON)
	{
		(*current_player)->won += 1;
		if ((*current_player)->lost != 0)
			(*current_player)->ratio = (double)(*current_player)->won / (double)(*current_player)->lost;
		else
			(*current_player)->ratio = -1;
	}
	else if (gamestat == PLAYER_LOST)
	{
		(*current_player)->lost += 1;
		(*current_player)->ratio = (double)(*current_player)->won / (double)(*current_player)->lost;
	}
	goto return_ret;

main_cleanup:
	free(*current_player);
return_ret:
	return ret;
}


int InsertPlayer(leaderboard_player** current_player, leaderboard_player **first_p)
{
	leaderboard_player *search_p = *first_p;
	leaderboard_player *search_p_before = NULL;

	//if the list is empty
	if (*first_p == NULL)
	{
		*first_p = *current_player;
		return 0;
	}

	//going over the list
	while (search_p != NULL)
	{
		//if the player has the best ratio or it has better ratio then the one we are in
		if (((*current_player)->ratio == -1) || (((*current_player)->ratio > search_p->ratio) && (search_p->ratio != -1)))
		{
			//if the current player is the first player in the list
			if (search_p_before == NULL)
				(*first_p) = *current_player;
			else
				search_p_before->next = *current_player;
			(*current_player)->next = search_p;
			return 0;
		}
		search_p_before = search_p;
		search_p = search_p->next;
	}
	//if the player is in the last place
	search_p_before->next = *current_player;
	return 0;
}


int WriteToFile(leaderboard_player** first_p)
{
	leaderboard_player *cur = *first_p;
	int err = 0, ret = 0;

	FILE *fp = NULL;

	err = fopen_s(&fp, "Leaderboard.csv", "w");
	if (err != 0)
	{
		printf("Error: opening leaderboard file\n");
		ret = ERROR_CODE;
		goto return_ret;
	}
	//first line in csv
	fputs("Name,Won,Lost,W/L Ratio\n", fp);

	while (cur != NULL)
	{
		if (cur->ratio != 0 && cur->ratio != -1)
		{
			err = fprintf_s(fp, "%s,%d,%d,%.3lf\n", cur->username, cur->won, cur->lost, cur->ratio);
			if (err <= 0)
			{
				printf("Error: writing to file\n");
				ret = ERROR_CODE;
				goto close_file;
			}
		}
		else
		{
			err = fprintf_s(fp, "%s,%d,%d\n", cur->username, cur->won, cur->lost);
			if (err <= 0)
			{
				printf("Error: writing to file\n");
				ret = ERROR_CODE;
				goto close_file;
			}
		}
		cur = cur->next;

	}

close_file:
	fclose(fp);
return_ret:
	return ret;
}


int UpdateLeaderboard(char *username, int gamestat)
{
	int err = 0;
	DWORD wait_code;
	int retVal = 0, ret = 0;
	leaderboard_player *current = NULL;

	wait_code = WaitForSingleObject(leaderboard_mutex, INFINITE);
	if (WAIT_OBJECT_0 != wait_code)
	{
		printf("Error when waiting for leaderboard_mutex\n");
		return ERROR_CODE;
	}
	/* Critical section */
	err = SearchInList(&current, username, gamestat, &first_player);
	if (err == ERROR_CODE) { ret = err; goto release; }

	err = InsertPlayer(&current, &first_player);

	err = WriteToFile(&first_player);
	if (err == ERROR_CODE) { ret = err; goto release; }

release:
	retVal = ReleaseMutex(leaderboard_mutex);
	if (FALSE == retVal)
	{
		printf("Error when releasing leaderboard_mutex\n");
		return ERROR_CODE;
	}

	return err;
}


int LeaderboardRead(char **leaderboard_message, int *leaderboard_length)
{
	//variables
	int line_length = 0, err = 0, ret = 0;
	int message_length = 0, first_line = 1;

	char *temp_message = NULL;
	char read_line[MAX_LINE] = "";
	FILE *fp = NULL;

	err = fopen_s(&fp, "Leaderboard.csv", "r");
	if (fp == NULL)
	{
		printf("Error: reading from leaderboard file\n");
		ret = ERROR_CODE;
		goto return_ret;
	}

	//reading the lines from the file
	while (fgets(read_line, MAX_LINE, fp) != NULL)
	{
		line_length = strlen(read_line);
		message_length = message_length + line_length;
		//if we read the first line
		if (first_line == 1)
		{
			//allocating first time for the message
			*leaderboard_message = (char*)malloc(message_length + 1);
			if (*leaderboard_message == NULL)
			{
				printf("Error: allocating memory for the message\n");
				ret = ERROR_CODE;
				goto close_file;
			}
			//copy the line from the file to the message info
			err = strncpy_s(*leaderboard_message, message_length + 1, read_line, message_length);
			if (err != 0)
			{
				printf("Error: string copy for the message info\n");
				ret = ERROR_CODE;
				goto free_message;
			}
			first_line = 0;
		}
		else
		{
			//reallocating the message info
			temp_message = (char*)realloc(*leaderboard_message, message_length + 1);
			if (temp_message == NULL) {
				printf("Error: reallocating memory for the message info\n");
				ret = ERROR_CODE;
				goto free_message;
			}
			//we used temp_message to not lose the message_info if failed
			*leaderboard_message = temp_message;
			//copy the new line from the file to the exist message info
			err = strncat_s(*leaderboard_message, message_length + 1, read_line, line_length);
			if (err != 0)
			{
				printf("Error: string copy for the message info\n");
				ret = ERROR_CODE;
				goto free_message;
			}
			first_line = 0;
		}
	}
	*leaderboard_length = strlen(*leaderboard_message) + 1;
	goto close_file;

free_message:
	free(*leaderboard_message);
close_file:
	fclose(fp);
return_ret:
	return ret;
}


int SendLeaderboardMessage(SOCKET sock, char **info, int info_length, char **saved)
{
	sendthread_s packet;
	char *message_send = NULL;

	int ret = 0, err = 0, exit_code = 0;

	packet.sock = sock;

	//first we will allocate memory for the message to be sent
	message_send = (char*)malloc(info_length + MAX_MESSAGE);
	if (message_send == NULL)
	{
		printf("Error: allocating memory to send the leaderboard message\n");
		ret = ERROR_CODE;
		goto return_ret;
	}

	//there is no saved leaderboard
	if (*saved == NULL)
	{
		err = sprintf_s(message_send, MAX_MESSAGE + info_length, "%s:%s\n", SERVER_LEADERBOARD, (*info));
		if (err == 0 || err == EOF)
		{
			printf("Error: can't create the message for the client\n");
			ret = ERROR_CODE;
			goto free_message_send;
		}

		//update the saved leaderboard to the new one
		*saved = *info;
		*info = NULL;
	}
	else
	{
		//if the saved leaderboard is the new one
		if (STRINGS_ARE_EQUAL(*info, *saved))
		{
			//we send to the client that the leaderboard hasn't changed
			err = sprintf_s(message_send, MAX_MESSAGE + info_length, "%s:%s\n", SERVER_LEADERBOARD, "The Leaderboard hasn't changed");
			if (err == 0 || err == EOF)
			{
				printf("Error: can't create the message for the client\n");
				ret = ERROR_CODE;
				goto free_message_send;
			}

			//free the new one and reset it to null
			free(*info);
			*info = NULL;
		}
		else
		{
			//the saved doesnt match the new one, and send the new one
			err = sprintf_s(message_send, MAX_MESSAGE + info_length, "%s:%s\n", SERVER_LEADERBOARD, (*info));
			if (err == 0 || err == EOF)
			{
				printf("Error: can't create the message for the client\n");
				ret = ERROR_CODE;
				goto free_message_send;
			}

			//free the old one and save the new one
			free(*saved);
			*saved = *info;
			*info = NULL;
		}
	}
	packet.array_t = message_send;
	packet.array_size = strlen(message_send);

	/*------------------------- send SERVER_LEADERBOARD ---------------------------------------*/
	//activate the send thread and get his exit code
	exit_code = ActivateThread((void*)&packet, 1, SENDRECV_WAITTIME);
	//if the thread setup failed or the thread function itself failed
	if (exit_code != 0) { ret = exit_code;  goto free_message_send; }

	/*------------------------- send SERVER_LEADERBOARD_MENU ----------------------------------*/
	err = sprintf_s(message_send, MAX_MESSAGE + info_length, "%s\n", SERVER_LEADERBOARD_MENU);
	if (err == 0 || err == EOF)
	{
		printf("Error: can't create the message for the client\n");
		ret = ERROR_CODE;
		goto free_message_send;
	}
	packet.array_t = message_send;
	packet.array_size = strlen(message_send);

	//activate the send thread and get his exit code
	exit_code = ActivateThread((void*)&packet, 1, SENDRECV_WAITTIME);
	//if the thread setup failed or the thread function itself failed
	if (exit_code != 0) { ret = exit_code;  goto free_message_send; }


free_message_send:
	free(message_send);
return_ret:
	return ret;
}


int LeaderBoardSelection(SOCKET sock)
{
	//variables
	int replay = 1, err = 0, wait_code = 0;
	int ret = 0, exit_code;
	char *leaderboard_info = NULL;
	char *leaderboard_saved = NULL;

	char message_type[MAX_MESSAGE];
	char message_info[MAX_MESSAGE];

	int leaderboard_length = 0;

	sendthread_s packet;
	packet.sock = sock;

	while (replay == 1)
	{
		//mutex to read the file
		wait_code = WaitForSingleObject(leaderboard_mutex, INFINITE);
		if (WAIT_OBJECT_0 != wait_code)
		{
			printf("Error when waiting for leaderboard_mutex\n");
			ret = ERROR_CODE;
			goto free_strings;
		}

		/* Critical section */
		err = LeaderboardRead(&leaderboard_info, &leaderboard_length);

		//release the mutex
		exit_code = ReleaseMutex(leaderboard_mutex);
		if (FALSE == exit_code)
		{
			printf("Error when releasing leaderboard_mutex\n");
			ret = ERROR_CODE;
			goto free_strings;
		}

		if (err == ERROR_CODE) { ret = ERROR_CODE; goto free_strings; }

		//send the leaderboard message based on the saved leaderboard
		err = SendLeaderboardMessage(sock, &leaderboard_info, leaderboard_length, &leaderboard_saved);
		if (err == ERROR_CODE) { ret = ERROR_CODE; goto free_strings; }

		/*-------------------------------recv leaderboard menu selection ---------------------------------*/
		packet.array_t = NULL;
		packet.array_size = 0;

		//activate the recv thread and get his exit code
		exit_code = ActivateThread((void*)&packet, 0, USER_WAITTIME);
		//if the thread setup failed or the thread function itself failed
		if (exit_code != 0) { ret = exit_code;  goto free_strings; }

		//get the message type and the information
		err = MessageCut(packet.array_t, packet.array_size, message_type, message_info);
		if (err == ERROR_CODE) { ret = ERROR_CODE; goto free_packet_array; }

		if (STRINGS_ARE_EQUAL(message_type, CLIENT_REFRESH))
			replay = 1;
		else if (STRINGS_ARE_EQUAL(message_type, CLIENT_MAIN_MENU))
			replay = 0;
		else
		{
			printf("Error: message recieved from the client doesnt match with the protocol\n");
			ret = ERROR_CODE;
			goto free_packet_array;
		}
		free(packet.array_t);
	}

	goto free_strings;

free_packet_array:
	free(packet.array_t);
free_strings:
	if (leaderboard_info != NULL)
		free(leaderboard_info);
	if (leaderboard_saved != NULL)
		free(leaderboard_saved);
return_ret:
	return ret;
}