#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include <time.h>
#include <pwd.h>
#include <sys/types.h>
#include <string.h>
#define PATHLENGTH 255

char get_file_mode(struct stat* stat_ptr);
char* get_file_permission(struct stat* stat_ptr);
int main(int argc, char** argv){
	char* USAGE = "USAGE: ls [directory name]";
	if(argc > 2){
		printf("%s\n",USAGE);
		return 0;
	}
	char* workingdir;
	char full_path[PATHLENGTH];
	if(argc == 1){
		workingdir = ".";
	}
	if(argc == 2){
		workingdir = argv[1];
	}
	DIR* dirptr = opendir(workingdir);
	if(dirptr == NULL){
		printf("Error:cannot open file");
		return 0;
	}
	struct dirent* subfile;
	printf("%-13s%-4s%-17s%-17s%-10s%-15s%-15s\n", "permission", "lnk", "owner", "group", "size", "time", "path");
	while((subfile = readdir(dirptr)) != NULL){	
		//concatenate working directory and file inside
		sprintf(full_path, "%s/%s", workingdir, subfile->d_name);
		struct stat filestat;
		stat(full_path, &filestat);
		char time_buffer[26];
		ctime_r(&filestat.st_mtime, time_buffer);
		struct passwd* pass_struct;
		//get user name from userid
		char user_owner[16 + 1];
		pass_struct = getpwuid(filestat.st_uid);
		if(pass_struct == NULL)	
			user_owner[0] = '\0';
		else {
			strncpy(user_owner, pass_struct->pw_name, 16);
			user_owner[16] = '\0';	
		}
		//get group name from groupid
		char group_owner[16 + 1];
		pass_struct = getpwuid(filestat.st_gid);
		if(pass_struct == NULL)
			group_owner[0] = '\0';
		else{ 	
			strncpy(group_owner, pass_struct->pw_name, 16);
			group_owner[16] = '\0';
		}
		time_buffer[16] = '\0';
		printf("%c%-12s%-4d%-17s%-17s%-10d%-15s%-15s\n", get_file_mode(&filestat), 
			get_file_permission(&filestat), (int) filestat.st_nlink,
			user_owner, group_owner,  (int) filestat.st_size, time_buffer + 4, subfile->d_name);	
	}
	return 0;
	
}

char get_file_mode(struct stat* stat_ptr){
	if(S_ISBLK(stat_ptr->st_mode)) return 'b';
	if(S_ISCHR(stat_ptr->st_mode)) return 'c';
	if(S_ISDIR(stat_ptr->st_mode)) return 'd';
	if(S_ISLNK(stat_ptr->st_mode)) return 'l';
	if(S_ISFIFO(stat_ptr->st_mode)) return 'p';
	if(S_ISREG(stat_ptr->st_mode)) return '-';
	//if(S_ISSOCK(stat_ptr->st_mode)) return 's';
	else return '-';
}
char file_permission[10];
char* get_file_permission(struct stat* stat_ptr){
	file_permission[0] = (stat_ptr->st_mode & S_IRUSR) ? 'r' : '-';
	file_permission[1] = (stat_ptr->st_mode & S_IWUSR) ? 'w' : '-';
	file_permission[2] = (stat_ptr->st_mode & S_IXUSR) ? 'x' : '-';
	file_permission[3] = (stat_ptr->st_mode & S_IRGRP) ? 'r' : '-';
	file_permission[4] = (stat_ptr->st_mode & S_IWGRP) ? 'w' : '-';
	file_permission[5] = (stat_ptr->st_mode & S_IXGRP) ? 'x' : '-';
	file_permission[6] = (stat_ptr->st_mode & S_IROTH) ? 'r' : '-';
	file_permission[7] = (stat_ptr->st_mode & S_IWOTH) ? 'w' : '-';
	file_permission[8] = (stat_ptr->st_mode & S_IXOTH) ? 'x' : '-';
	file_permission[9] = '\0';

	return file_permission;
}
