#pragma once

class Privilege {
	void DisplayError(TCHAR* pszAPI, DWORD dwError) {
		UNREFERENCED_PARAMETER(pszAPI);
		LPVOID lpvMessageBuffer;

		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, dwError,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&lpvMessageBuffer, 0, NULL);

		//... now display this string
#ifdef _DEBUG
		_tprintf(TEXT("ERROR: API        = %s\n"), pszAPI);
		_tprintf(TEXT("       error code = %d\n"), dwError);
		_tprintf(TEXT("       message    = %s\n"), (LPTSTR)lpvMessageBuffer);
#endif

		// Free the buffer allocated by the system
		LocalFree(lpvMessageBuffer);

		//ExitProcess(GetLastError());
	}

public:
	Privilege(TCHAR* pszPrivilege, BOOL bEnable = TRUE) {
		HANDLE           hToken;
		TOKEN_PRIVILEGES tp;
		BOOL             status;
		DWORD            error;

		// open process token
		if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
			DisplayError(TEXT("OpenProcessToken"), GetLastError());

		// get the luid
		if (!LookupPrivilegeValue(NULL, pszPrivilege, &tp.Privileges[0].Luid))
			DisplayError(TEXT("LookupPrivilegeValue"), GetLastError());

		tp.PrivilegeCount = 1;

		// enable or disable privilege
		if (bEnable)
			tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		else
			tp.Privileges[0].Attributes = 0;

		// enable or disable privilege
		status = AdjustTokenPrivileges(hToken, FALSE, &tp, 0, (PTOKEN_PRIVILEGES)NULL, 0);

		// It is possible for AdjustTokenPrivileges to return TRUE and still not succeed.
		// So always check for the last error value.
		error = GetLastError();
		if (!status || (error != ERROR_SUCCESS))
			DisplayError(TEXT("AdjustTokenPrivileges"), GetLastError());

		// close the handle
		if (!CloseHandle(hToken))
			DisplayError(TEXT("CloseHandle"), GetLastError());
	}

};

#if 0
SE_ASSIGNPRIMARYTOKEN_NAME
TEXT("SeAssignPrimaryTokenPrivilege")
Required to assign the primary token of a process.
User Right : Replace a process - level token.

SE_AUDIT_NAME
TEXT("SeAuditPrivilege")
Required to generate audit - log entries.Give this privilege to secure servers.
User Right : Generate security audits.

SE_BACKUP_NAME
TEXT("SeBackupPrivilege")
Required to perform backup operations.This privilege causes the system to grant all read access control to any file, regardless of the access control list(ACL) specified for the file.Any access request other than read is still evaluated with the ACL.This privilege is required by the RegSaveKey and RegSaveKeyExfunctions.The following access rights are granted if this privilege is held :
READ_CONTROL
ACCESS_SYSTEM_SECURITY
FILE_GENERIC_READ
FILE_TRAVERSE
User Right : Back up files and directories.


SE_CHANGE_NOTIFY_NAME
TEXT("SeChangeNotifyPrivilege")
Required to receive notifications of changes to files or directories.This privilege also causes the system to skip all traversal access checks.It is enabled by default for all users.
User Right : Bypass traverse checking.

#endif

class SeCreateGlobalPrivilege : public Privilege { public: SeCreateGlobalPrivilege() : Privilege{ SE_CREATE_GLOBAL_NAME, TRUE } { } };
//SE_CREATE_GLOBAL_NAME
//TEXT("SeCreateGlobalPrivilege")
//Required to create named file mapping objects in the global namespace during Terminal Services sessions.This privilege is enabled by default for administrators, services, and the local system account.
//User Right : Create global objects.

#if 0

SE_CREATE_PAGEFILE_NAME
TEXT("SeCreatePagefilePrivilege")
Required to create a paging file.
User Right : Create a pagefile.


SE_CREATE_PERMANENT_NAME
TEXT("SeCreatePermanentPrivilege")
Required to create a permanent object.
User Right : Create permanent shared objects.


SE_CREATE_SYMBOLIC_LINK_NAME
TEXT("SeCreateSymbolicLinkPrivilege")
Required to create a symbolic link.
User Right : Create symbolic links.


SE_CREATE_TOKEN_NAME
TEXT("SeCreateTokenPrivilege")
Required to create a primary token.
User Right : Create a token object.
You cannot add this privilege to a user account with the "Create a token object" policy.Additionally, you cannot add this privilege to an owned process using Windows APIs.
Windows Server 2003 and Windows XP with SP1 and earlier : Windows APIs can add this privilege to an owned process.


SE_DEBUG_NAME
TEXT("SeDebugPrivilege")
Required to debug and adjust the memory of a process owned by another account.
User Right : Debug programs.


SE_ENABLE_DELEGATION_NAME
TEXT("SeEnableDelegationPrivilege")
Required to mark user and computer accounts as trusted for delegation.
User Right : Enable computer and user accounts to be trusted for delegation.


SE_IMPERSONATE_NAME
TEXT("SeImpersonatePrivilege")
Required to impersonate.
User Right : Impersonate a client after authentication.


SE_INC_BASE_PRIORITY_NAME
TEXT("SeIncreaseBasePriorityPrivilege")
Required to increase the base priority of a process.
User Right : Increase scheduling priority.


SE_INCREASE_QUOTA_NAME
TEXT("SeIncreaseQuotaPrivilege")
Required to increase the quota assigned to a process.
User Right : Adjust memory quotas for a process.


SE_INC_WORKING_SET_NAME
TEXT("SeIncreaseWorkingSetPrivilege")
Required to allocate more memory for applications that run in the context of users.
User Right : Increase a process working set.


SE_LOAD_DRIVER_NAME
TEXT("SeLoadDriverPrivilege")
Required to load or unload a device driver.
User Right : Load and unload device drivers.

#endif

class SeLockMemoryPrivilege : public Privilege { public: SeLockMemoryPrivilege() : Privilege{ SE_LOCK_MEMORY_NAME, TRUE } { } };
//SE_LOCK_MEMORY_NAME
//TEXT("SeLockMemoryPrivilege")
//Required to lock physical pages in memory.
//User Right : Lock pages in memory.

#if 0
SE_MACHINE_ACCOUNT_NAME
TEXT("SeMachineAccountPrivilege")
Required to create a computer account.
User Right : Add workstations to domain.


SE_MANAGE_VOLUME_NAME
TEXT("SeManageVolumePrivilege")
Required to enable volume management privileges.
User Right : Manage the files on a volume.


SE_PROF_SINGLE_PROCESS_NAME
TEXT("SeProfileSingleProcessPrivilege")
Required to gather profiling information for a single process.
User Right : Profile single process.


SE_RELABEL_NAME
TEXT("SeRelabelPrivilege")
Required to modify the mandatory integrity level of an object.
User Right : Modify an object label.


SE_REMOTE_SHUTDOWN_NAME
TEXT("SeRemoteShutdownPrivilege")
Required to shut down a system using a network request.
User Right : Force shutdown from a remote system.


SE_RESTORE_NAME
TEXT("SeRestorePrivilege")
Required to perform restore operations.This privilege causes the system to grant all write access control to any file, regardless of the ACL specified for the file.Any access request other than write is still evaluated with the ACL.Additionally, this privilege enables you to set any valid user or group SID as the owner of a file.This privilege is required by the RegLoadKey function.The following access rights are granted if this privilege is held :
WRITE_DAC
WRITE_OWNER
ACCESS_SYSTEM_SECURITY
FILE_GENERIC_WRITE
FILE_ADD_FILE
FILE_ADD_SUBDIRECTORY
DELETE
User Right : Restore files and directories.


SE_SECURITY_NAME
TEXT("SeSecurityPrivilege")
Required to perform a number of security - related functions, such as controlling and viewing audit messages.This privilege identifies its holder as a security operator.
User Right : Manage auditing and security log.


SE_SHUTDOWN_NAME
TEXT("SeShutdownPrivilege")
Required to shut down a local system.
User Right : Shut down the system.


SE_SYNC_AGENT_NAME
TEXT("SeSyncAgentPrivilege")
Required for a domain controller to use the Lightweight Directory Access Protocol directory synchronization services.This privilege enables the holder to read all objects and properties in the directory, regardless of the protection on the objects and properties.By default, it is assigned to the Administrator and LocalSystem accounts on domain controllers.
User Right : Synchronize directory service data.

SE_SYSTEM_ENVIRONMENT_NAME
TEXT("SeSystemEnvironmentPrivilege")
Required to modify the nonvolatile RAM of systems that use this type of memory to store configuration information.
User Right : Modify firmware environment values.


SE_SYSTEM_PROFILE_NAME
TEXT("SeSystemProfilePrivilege")
Required to gather profiling information for the entire system.
User Right : Profile system performance.


SE_SYSTEMTIME_NAME
TEXT("SeSystemtimePrivilege")
Required to modify the system time.
User Right : Change the system time.


SE_TAKE_OWNERSHIP_NAME
TEXT("SeTakeOwnershipPrivilege")
Required to take ownership of an object without being granted discretionary access.This privilege allows the owner value to be set only to those values that the holder may legitimately assign as the owner of an object.
User Right : Take ownership of files or other objects.


SE_TCB_NAME
TEXT("SeTcbPrivilege")
This privilege identifies its holder as part of the trusted computer base.Some trusted protected subsystems are granted this privilege.
User Right : Act as part of the operating system.


SE_TIME_ZONE_NAME
TEXT("SeTimeZonePrivilege")
Required to adjust the time zone associated with the computer's internal clock.
User Right : Change the time zone.


SE_TRUSTED_CREDMAN_ACCESS_NAME
TEXT("SeTrustedCredManAccessPrivilege")
Required to access Credential Manager as a trusted caller.
User Right : Access Credential Manager as a trusted caller.


SE_UNDOCK_NAME
TEXT("SeUndockPrivilege")
Required to undock a laptop.
User Right : Remove computer from docking station.


SE_UNSOLICITED_INPUT_NAME
TEXT("SeUnsolicitedInputPrivilege")
Required to read unsolicited input from a terminal device.
User Right : Not applicable.
#endif