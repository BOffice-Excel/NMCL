#include "Json.h"
#include <errno.h>
#include <string.h>
#include <urlmon.h>
#include <windows.h>
LPJSONOBJ lpJsonObj,lpTmp;
JsonObjectType FT;
char FileText[1145141], VersionPath[1145], *CreateDirs[32768], *DownloadFiles[32768][2], Tmp[32768], VersionName[128], Arg[114514], AssetsVer[256], mainClass[256], HerePath[32768], ClassPath[32768], NativesDirectory[32768];
int cCreateDirs, cDownloads, JavaVersion;
void PushDir(LPCSTR Format,...){
	va_list valst;
	va_start(valst,Format);
	CreateDirs[cCreateDirs] = (char*)calloc(256, sizeof(char));
	vsprintf(CreateDirs[cCreateDirs++],Format,valst);
	va_end(valst);
}
void PushFile(LPCSTR From, LPCSTR Format, ...){
	va_list valst;
	va_start(valst,Format);
	DownloadFiles[cDownloads][0] = (char*)calloc(strlen(From) + 10, sizeof(char));
	strcpy(DownloadFiles[cDownloads][0], From);
	DownloadFiles[cDownloads][1] = (char*)calloc(256, sizeof(char));
	vsprintf(DownloadFiles[cDownloads++][1], Format, valst);
	va_end(valst);
}
void Replacer(LPSTR lpstrIn){
	LPSTR Res=(LPSTR)calloc(114514,sizeof(char));
	int x=0,i;
	for(i=0;i<strlen(lpstrIn);i++){
		if(lpstrIn[i] == '$' && lpstrIn[i + 1] == '{') {
			i += 2;
			if(strncmp(lpstrIn + i, "version_name}", 13) == 0) {
				strcat(Res, VersionName);
				i += 12;
			}
			else if(strncmp(lpstrIn + i, "version_path}", 13) == 0) {
				strcat(Res, VersionPath);
				i += 12;
			}
			else if(strncmp(lpstrIn + i, "arguments}", 10) == 0) {
				strcat(Res, Arg);
				i += 9;
			}
			else if(strncmp(lpstrIn + i, "mainClass}", 10) == 0) {
				strcat(Res, mainClass);
				i += 9;
			}
			else if(strncmp(lpstrIn + i, "auth_player_name}", 17) == 0) {//Player Name
				strcat(Res, "Office-Excel");
				i += 16;
			}
			else if(strncmp(lpstrIn + i, "auth_uuid}", 10) == 0) {//Uuid
				//GUID guid;
				//sprintf(Res + strlen(Res), "%x-%x-%x-%x%x-%x%x%x", guid.Data1, guid.Data2, guid.Data3, guid.Data4[0]);
				strcat(Res, "8096a399524e4eaf831894b8b0a43ce7");//8096a399-524e-4eaf-831894b8b0a43ce7
				i += 9;
			}
			else if(strncmp(lpstrIn + i, "auth_access_token}", 18) == 0) {//Uuid
				//GUID guid;
				//sprintf(Res + strlen(Res), "%x-%x-%x-%x%x-%x%x%x", guid.Data1, guid.Data2, guid.Data3, guid.Data4[0]);
				strcat(Res, "8096a399524e4eaf831894b8b0a43ce7");//8096a399-524e-4eaf-831894b8b0a43ce7
				i += 17;
			}
			else if(strncmp(lpstrIn + i, "auth_session}", 13) == 0) {//Uuid
				//GUID guid;
				//sprintf(Res + strlen(Res), "%x-%x-%x-%x%x-%x%x%x", guid.Data1, guid.Data2, guid.Data3, guid.Data4[0]);
				strcat(Res, "8096a399524e4eaf831894b8b0a43ce7");//8096a399-524e-4eaf-831894b8b0a43ce7
				i += 12;
			}
			else if(strncmp(lpstrIn + i, "user_type}", 10) == 0) {//UserType
				strcat(Res, "msa");
				i += 9;
			}
			else if(strncmp(lpstrIn + i, "version_type}", 13) == 0) {//VersionType
				strcat(Res, "NMCL");
				i += 12;
			}
			else if(strncmp(lpstrIn + i, "assets_index_name}", 18) == 0) {//Assets Index Name
				strcat(Res, AssetsVer);
				i += 17;
			}
			else if(strncmp(lpstrIn + i, "game_directory}", 15) == 0) {//Game Dir
				strcat(Res, VersionPath);
				i += 14;
			}
			else if(strncmp(lpstrIn + i, "assets_root}", 12) == 0) {//Assets Dir
				strcat(Res, ".minecraft\\assets");
				i += 11;
			}
			else if(strncmp(lpstrIn + i, "game_assets}", 12) == 0) {//Assets Dir
				strcat(Res, ".minecraft\\assets");
				i += 11;
			}
			else if(strncmp(lpstrIn + i, "launcher_version}", 17) == 0) {//Launcher Version
				strcat(Res, "114.5.14");
				i += 16;
			}
			else if(strncmp(lpstrIn + i, "launcher_name}", 14) == 0) {//Launcher Name
				strcat(Res, "NMCL");
				i += 13;
			}
			else if(strncmp(lpstrIn + i, "classpath}", 10) == 0) {
				strcat(Res, ClassPath);
				i += 9;
			}
			else if(strncmp(lpstrIn + i, "natives_directory}", 18) == 0) {
				strcat(Res, NativesDirectory);
				i += 17;
			}
			else if(strncmp(lpstrIn + i, "user_properties}", 16) == 0) {
				strcat(Res, "{}");
				i += 15;
			}
			else {
				i -= 2;
				Res[x] = '$';
			}
		}
		else Res[x] = lpstrIn[i];
		x = strlen(Res);
	}
	strcpy(lpstrIn,Res);
	free(Res);
}
int main(){
	FILE* lpFile;
	GetModuleFileName(GetModuleHandle(0),HerePath,1145);
	int ThisIndex;
	for(ThisIndex=strlen(HerePath)-1;ThisIndex>=0;ThisIndex--){
		if(HerePath[ThisIndex]=='\\') break;
		else HerePath[ThisIndex]='\0';
	}
	HerePath[ThisIndex]='\0';
	CreateDirectory("NMCL", NULL);
	CreateDirectory(".minecraft", NULL);
	CreateDirectory(".minecraft\\assets", NULL);
	CreateDirectory(".minecraft\\assets\\indexes", NULL);
	CreateDirectory(".minecraft\\versions", NULL);
	sprintf(VersionPath, "%s\\NMCL\\version_manifest.json", HerePath);
	printf("Downloading version file version_manifest.json...");
	URLDownloadToFileA(NULL, "https://piston-meta.mojang.com/mc/game/version_manifest.json", VersionPath, 0, NULL);
	printf("\rDownloaded version file version_manifest.json successfully!\n");
	lpFile = fopen(VersionPath, "r");
	int size = fread(FileText, 1, sizeof(FileText), lpFile);
	fclose(lpFile);
	lpJsonObj = BuildJson(FileText, size, &FT);
	SelectVersion:
	lpTmp = lpJsonObj -> Next -> Childs;
	printf("Please choose the Minecraft version: \n");
	while(lpTmp != NULL) {
		printf("  %s(Type: %s)\n", lpTmp -> Childs -> lpstrValue, lpTmp -> Childs -> Next -> lpstrValue);
		lpTmp = lpTmp -> Next;
	} 
	printf("Please input the version name: ");
	scanf("%s", VersionName);
	lpTmp = lpJsonObj -> Next -> Childs;
	printf("Finding Version from name...");
	while(lpTmp != NULL) {
		if(strcmp(lpTmp -> Childs -> lpstrValue, VersionName) == 0) {
			printf("\rVersion %s Found\n", VersionName);
			break;
		}
		lpTmp = lpTmp -> Next;
	} 
	if(lpTmp == NULL) {
		printf("ERROR: Non-existent version %s\n", VersionName);
		goto SelectVersion;
	}
	sprintf(VersionPath, "%s\\.minecraft\\versions\\%s\\%s.json", HerePath, VersionName, VersionName);
	printf("Downloading version manifest file %s...", lpTmp -> Childs -> Next -> Next -> lpstrValue);
	URLDownloadToFileA(NULL, lpTmp -> Childs -> Next -> Next -> lpstrValue, VersionPath, 0, NULL);
	printf("\rDownloaded version manifest file %s.json successfully! File Path: %s\n", VersionName, VersionPath);
	CloseJson(lpJsonObj);
	lpFile = fopen(VersionPath/*"MC1.21.4.json"*/, "r");
	memset(VersionPath, 0, sizeof(VersionPath));
	memset(VersionName, 0, sizeof(VersionName));
	if(lpFile == NULL) {
		printf("ERROR: File Opened failed! Path: %s", VersionPath, strerror(errno));
		return 0;
	} 
	printf("File opened successfully!\n");
	memset(FileText, 0, sizeof(FileText));
	size = fread(FileText, 1, sizeof(FileText), lpFile);
	fclose(lpFile);
	FT = (JsonObjectType)0;
	lpTmp = lpJsonObj = BuildJson(FileText, size, &FT);
	printf("Built json successfully!\n");
	PushDir(".minecraft");
	PushDir(".minecraft\\versions");
	PushDir(".minecraft\\assets");
	PushDir(".minecraft\\assets\\objects");
	PushDir(".minecraft\\libraries");
	PushDir("NMCL");
	// --add-exports cpw.mods.bootstraplauncher/cpw.mods.bootstraplauncher=ALL-UNNAMED
	sprintf(Arg, "-Dstderr.encoding=UTF-8 -Dstdout.encoding=UTF-8 -Dfile.encoding=COMPAT -Djdk.lang.Process.allowAmbiguousCommands=true -Dfml.ignoreInvalidMinecraftCertificates=True -Dfml.ignorePatchDiscrepancies=True -Dlog4j2.formatMsgNoLookups=true -Xmx%dm -Xmn%dm -XX:+UseG1GC -XX:-UseAdaptiveSizePolicy -XX:-OmitStackTraceInFastThrow -XX:HeapDumpPath=MojangTricksIntelDriversForPerformance_javaw.exe_minecraft.exe.heapdump -Dos.name=\"Windows 10\" -Dos.version=10.0", 1024, 128);
	while(lpTmp!=NULL){
		printf("Read key %s\n", lpTmp->lpstrKey);
		if(strcmp(lpTmp->lpstrKey,"downloads")==0){
			LPJSONOBJ lpDownloads=lpTmp,lpClient=lpDownloads->Childs;
			PushFile(lpClient->Childs->Next->Next->lpstrValue,"${version_path}\\${version_name}.jar");
		}
		else if(strcmp(lpTmp -> lpstrKey, "id") == 0) {
			strcpy(VersionName,lpTmp->lpstrValue);
			sprintf(VersionPath,"%s\\.minecraft\\versions\\%s",HerePath,lpTmp->lpstrValue);
			PushDir(VersionPath);
			CreateDirectory(VersionPath, NULL);
			sprintf(NativesDirectory, "%s\\.minecraft\\versions\\%s\\%s-natives", HerePath, lpTmp -> lpstrValue, lpTmp -> lpstrValue);
			PushDir(NativesDirectory);
			CreateDirectory(NativesDirectory, NULL);
		}
		else if(strcmp(lpTmp->lpstrKey,"mainClass")==0){
			strcpy(mainClass,lpTmp->lpstrValue);
		}
		else if(strcmp(lpTmp -> lpstrKey, "arguments") == 0) {
			LPJSONOBJ lpArgGame = lpTmp -> Childs -> Childs, Game;
			if(strcmp(lpTmp -> Childs -> lpstrKey, "game") == 0) Game = lpArgGame, lpArgGame = lpTmp -> Childs -> Next -> Childs;
			while(lpArgGame != NULL) {
				if(lpArgGame -> iMask == OT_LSTCHILDSTR) {
					//int Backup = strlen(Arg);
					strcat(Arg, " ");
					strcat(Arg, lpArgGame -> lpstrValue);
					printf("%s %s\n", lpArgGame -> lpstrValue, lpArgGame -> lpstrKey);
					//strcat(Arg, " ");
					//lpArgGame=lpArgGame->Next;
					//strcat(Arg, lpArgGame->lpstrValue);
				}
				/*else{
					LPJSONOBJ lpChilds=lpArgGame->Childs;
					
				}//*/
				lpArgGame = lpArgGame -> Next;
			}
			strcat(Arg, " ${mainClass}");
			lpArgGame = Game;
			while(lpArgGame!=NULL){
				if(lpArgGame->iMask==OT_LSTCHILDSTR){
					strcat(Arg, " ");
					strcat(Arg, lpArgGame->lpstrValue);
				}
				lpArgGame=lpArgGame->Next;
			}
		}
		else if(strcmp(lpTmp->lpstrKey,"libraries")==0){
			LPJSONOBJ lpLib=lpTmp->Childs;
			while(lpLib!=NULL){
				LPJSONOBJ lpDownloads=lpLib;
				LPJSONOBJ lpTmp = lpLib -> Childs;
				int AllowWindows = 1;
				while(lpTmp != NULL) {
					if(strcmp(lpTmp -> lpstrKey, "rules") == 0) {
						lpTmp = lpTmp -> Childs;
						AllowWindows = 0;
						while(lpTmp != NULL) {
							if(strcmp(lpTmp -> Childs -> lpstrValue, "allow") == 0 && lpTmp -> Childs -> Next == NULL) {
								AllowWindows = 1;
								break;
							}
							if(strcmp(lpTmp -> Childs -> lpstrValue, "allow") == 0 && strcmp(lpTmp -> Childs -> Next -> Childs -> lpstrValue, "windows") == 0) {
								AllowWindows = 1;
								break;
							}
							
							lpTmp = lpTmp -> Next;
						}
						break;
					}
					if(strcmp(lpTmp -> lpstrKey, "name") == 0) printf("Solving library %s...\n", lpTmp -> lpstrValue); 
					lpTmp = lpTmp -> Next;
				}
				if(AllowWindows == 0) {
					lpLib=lpLib->Next;
					continue;
				}
				int i,Worked=0;
				/*for(i=0;i<strlen(lpDownloads->Childs->Next->lpstrValue);i++){
					if(lpDownloads->Childs->Next->lpstrValue[i]==':') Worked=1;
					if(lpDownloads->Childs->Next->lpstrValue[i]==':'||lpDownloads->Childs->Next->lpstrValue[i]=='.'&&Worked==0){
						lpDownloads->Childs->Next->lpstrValue[i]='\0';
						PushDir(".minecraft\\libraries\\%s",lpDownloads->Childs->Next->lpstrValue);
						lpDownloads->Childs->Next->lpstrValue[i]='\\';
					}
				}*/
				printf("Pushing Directory...\n");
				for(i=0;i<strlen(lpDownloads->Childs->Childs->Childs->lpstrValue);i++){
					if(lpDownloads->Childs->Childs->Childs->lpstrValue[i]=='/'){
						lpDownloads->Childs->Childs->Childs->lpstrValue[i]='\0';
						PushDir("%s\\.minecraft\\libraries\\%s",HerePath,lpDownloads->Childs->Childs->Childs->lpstrValue);
						//CreateDirectory(lpDownloads->Childs->Childs->Childs->lpstrValue,NULL);
						//printf("Created: %s\n",lpDownloads->Childs->Childs->Childs->lpstrValue);
						lpDownloads->Childs->Childs->Childs->lpstrValue[i]='\\';
					}
				}
				printf("Pushing file...\n");
				if(strcmp(lpDownloads -> Childs -> Childs -> lpstrKey, "artifact") == 0) {
					PushFile(lpDownloads->Childs->Childs->Childs->Next->Next->Next->lpstrValue,"%s\\.minecraft\\libraries\\%s",HerePath,lpDownloads->Childs->Childs->Childs->lpstrValue);
					if(strcmp(lpDownloads->Childs->Childs->Childs->lpstrValue + strlen(lpDownloads->Childs->Childs->Childs->lpstrValue) - 10, "-linux.jar") != 0
					&& strcmp(lpDownloads->Childs->Childs->Childs->lpstrValue + strlen(lpDownloads->Childs->Childs->Childs->lpstrValue) - 10, "-macos.jar") != 0
					&& strcmp(lpDownloads->Childs->Childs->Childs->lpstrValue + strlen(lpDownloads->Childs->Childs->Childs->lpstrValue) - 16, "-macos-arm64.jar") != 0
					&& strcmp(lpDownloads->Childs->Childs->Childs->lpstrValue + strlen(lpDownloads->Childs->Childs->Childs->lpstrValue) - 16, "-windows-x86.jar") != 0
					&& strcmp(lpDownloads->Childs->Childs->Childs->lpstrValue + strlen(lpDownloads->Childs->Childs->Childs->lpstrValue) - 18, "-windows-arm64.jar") != 0
					) sprintf(ClassPath + strlen(ClassPath), "%s\\.minecraft\\libraries\\%s;", HerePath,lpDownloads->Childs->Childs->Childs->lpstrValue);
				}
				printf("Pushing Natives...\n");
				if(lpLib -> Childs -> Childs -> Next != NULL && lpLib -> Childs -> Childs -> Next -> lpstrKey != NULL && strcmp(lpLib -> Childs -> Childs -> Next -> lpstrKey, "classifiers") == 0) {
					LPJSONOBJ lpNatives = lpLib -> Childs -> Childs -> Next -> Childs;
					while(lpNatives != NULL) {
						if(strcmp(lpNatives -> lpstrKey, "natives-windows") == 0) {
							CreateDirectory("NMCL\\Temp", NULL);
							CreateDirectory("NMCL\\Temp\\natives", NULL);
							printf("Downloading from \"%s\" to \"%s\"...\n", lpNatives -> Childs -> Next -> Next -> Next -> lpstrValue, "NMCL\\Temp\\natives\\natives-windows.jar");
							URLDownloadToFile(NULL, lpNatives -> Childs -> Next -> Next -> Next -> lpstrValue, "NMCL\\Temp\\natives\\natives-windows.jar", 0, NULL);
							printf("Extracting File %s...\n", lpNatives -> Childs -> lpstrValue);
							sprintf(Tmp, "7z.exe x -o%s -y NMCL\\Temp\\natives\\natives-windows.jar *.dll", NativesDirectory);
							system(Tmp);
							break;
						}
						lpNatives = lpNatives -> Next;
					}
				}
				if(strcmp(lpLib -> Childs -> Childs -> lpstrKey, "classifiers") == 0) {
					LPJSONOBJ lpNatives = lpLib -> Childs -> Childs -> Childs;
					while(lpNatives != NULL) {
						if(strcmp(lpNatives -> lpstrKey, "natives-windows") == 0) {
							CreateDirectory("NMCL\\Temp", NULL);
							CreateDirectory("NMCL\\Temp\\natives", NULL);
							printf("Downloading from \"%s\" to \"%s\"...\n", lpNatives -> Childs -> Next -> Next -> Next -> lpstrValue, "NMCL\\Temp\\natives\\natives-windows.jar");
							URLDownloadToFile(NULL, lpNatives -> Childs -> Next -> Next -> Next -> lpstrValue, "NMCL\\Temp\\natives\\natives-windows.jar", 0, NULL);
							printf("Extracting File %s...\n", lpNatives -> Childs -> lpstrValue);
							sprintf(Tmp, "7z.exe x -o%s -y NMCL\\Temp\\natives\\natives-windows.jar *.dll", NativesDirectory);
							system(Tmp);
							break;
						}
						lpNatives = lpNatives -> Next;
					}
				}
				lpLib=lpLib->Next;
			}
		}
		else if(strcmp(lpTmp->lpstrKey,"assets")==0){
			strcpy(AssetsVer,lpTmp->lpstrValue);
		}
		else if(strcmp(lpTmp -> lpstrKey, "assetIndex") == 0) {
			LPJSONOBJ lpJsonUrl = lpTmp -> Childs;
			while(lpJsonUrl != NULL) {
				if(strcmp(lpJsonUrl -> lpstrKey, "id") == 0) {
					strcpy(AssetsVer, lpJsonUrl -> lpstrValue);
				}
				else if(strcmp(lpJsonUrl -> lpstrKey, "url") == 0) {
					sprintf(Tmp, "%s\\.minecraft\\assets\\indexes\\%s.json", HerePath, AssetsVer);
					printf("Downloading from \"%s\" to \"%s\"...\n", lpJsonUrl -> lpstrValue, Tmp);
					URLDownloadToFile(NULL, lpJsonUrl -> lpstrValue, Tmp, 0, NULL);
					lpFile = fopen(Tmp, "r");
					memset(FileText, 0, sizeof(FileText)); 
					int size = fread(FileText, 1, sizeof(FileText), lpFile);
					printf("Assets Index Json File Size: %d\n", size);
					fclose(lpFile);
					printf("%s\n", FileText);
					LPJSONOBJ lpObject, lpAssetsJson = BuildJson(FileText, size, NULL);
					printf("Assets Index Json File solved successfully!\n");
					lpObject = lpAssetsJson;
					while(lpObject != NULL && strcmp(lpObject -> lpstrKey, "objects") != 0) {
						lpObject = lpObject -> Next;
					}
					lpObject = lpObject -> Childs;
					while(lpObject != NULL) {
						PushDir("%s\\.minecraft\\assets\\objects\\%c%c", HerePath, lpObject -> Childs -> lpstrValue[0], lpObject -> Childs -> lpstrValue[1]);
						sprintf(Tmp, "https://resources.download.minecraft.net/%c%c/%s", lpObject -> Childs -> lpstrValue[0], lpObject -> Childs -> lpstrValue[1], lpObject -> Childs -> lpstrValue);
						PushFile(Tmp, "%s\\.minecraft\\assets\\objects\\%c%c\\%s", HerePath, lpObject -> Childs -> lpstrValue[0], lpObject -> Childs -> lpstrValue[1], lpObject -> Childs -> lpstrValue);
						lpObject = lpObject -> Next;
					}
					CloseJson(lpAssetsJson);
				}
				lpJsonUrl = lpJsonUrl -> Next;
			}
		}
		else if(strcmp(lpTmp -> lpstrKey, "minecraftArguments") == 0) {
			strcat(Arg, " -Djava.library.path=${natives_directory} -cp ${classpath} ${mainClass} ");
			strcat(Arg, lpTmp -> lpstrValue);
		}
		else if(strcmp(lpTmp -> lpstrKey, "javaVersion") == 0) {
			JavaVersion = lpTmp -> Childs -> Next -> Number;
		}
		lpTmp=lpTmp->Next;
	}
	CloseJson(lpJsonObj);
	printf("Version Manifest file analysis completed!\n");
	int i;
	for(i = 0; i < cCreateDirs; i++) {
		printf("Create Directory \"%s\"...\n", CreateDirs[i]);
		CreateDirectory(CreateDirs[i], NULL);
		free(CreateDirs[i]);
	}
	for(i=0;i<cDownloads;i++){
		Replacer(DownloadFiles[i][1]);
		printf("Downloading from \"%s\" to \"%s\"...\n", DownloadFiles[i][0], DownloadFiles[i][1]);
		Sleep(10);
		URLDownloadToFileA(NULL,DownloadFiles[i][0],DownloadFiles[i][1],0,NULL);
		free(DownloadFiles[i][0]);
		free(DownloadFiles[i][1]);
	}
	strcat(ClassPath, "${game_directory}\\${version_name}.jar");
	//ClassPath[strlen(ClassPath) - 1] = '\0';
	Replacer(ClassPath);
	Replacer(Arg);
	strcat(Arg, " --width 845 --height 480");
	printf("%s\n", Arg);
	char JavaExePath[32768];
	memset(JavaExePath, 0, sizeof(JavaExePath));
	OPENFILENAMEA ofn;
	memset(&ofn, 0, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFilter = "Java Runtime Environment/Java Development Kit(*.exe)\0*.exe\0\0";
	ofn.lpstrFile = JavaExePath;
	ofn.nMaxFile = sizeof(JavaExePath);
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST;
	SetCurrentDirectoryA(HerePath);
	if(GetOpenFileNameA(&ofn) == TRUE) {
		SetCurrentDirectoryA(HerePath);
		char Cmd[114514];
		sprintf(Cmd, "%s %s", JavaExePath, Arg);
		STARTUPINFOA si;
		PROCESS_INFORMATION pi;
		memset(&si, 0, sizeof si);
		memset(&pi, 0, sizeof pi);
		CreateProcessA(NULL, Cmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
		printf("Minecraft %s is starting(Process Id: %ld). Enjoy Your Time! : )\n", VersionName, pi.dwProcessId);
	}
	//WinExec(Cmd, SW_SHOW);
	return 0;
}
