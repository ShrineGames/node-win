#include "service.h"

int Input::GetTimeSinceLastInput()
{
  LASTINPUTINFO last_input_info {};
  last_input_info.cbSize = sizeof(LASTINPUTINFO);
  last_input_info.dwTime = 0;

  if (!GetLastInputInfo(&last_input_info)) {
    return -1;
  }

  int ticks = GetTickCount();
  int idle_ms = ticks - last_input_info.dwTime;

  return idle_ms;
}

bool Service::Install(const std::string& serviceName,
                      const std::string& displayName,
                      const std::string& fileName)
{
  auto manager = OpenManager(SC_MANAGER_CONNECT | SC_MANAGER_CREATE_SERVICE);
  if (manager == nullptr) {
    return false;
  }

  auto service = OpenService(
      manager, serviceName.c_str(), SERVICE_QUERY_STATUS | SERVICE_START);
  if (service == nullptr) {
    service = CreateService(manager,
                            serviceName.c_str(),
                            displayName.c_str(),
                            SERVICE_QUERY_STATUS | SERVICE_START,
                            SERVICE_WIN32_OWN_PROCESS,
                            SERVICE_AUTO_START,
                            SERVICE_ERROR_NORMAL,
                            fileName.c_str(),
                            nullptr,
                            nullptr,
                            nullptr,
                            nullptr,
                            nullptr);
  }

  if (service == nullptr) {
    printf("CreateService failed (%lu)\n", GetLastError());
    CloseServiceHandle(manager);
    return false;
  }

  bool started = Start(service);

  CloseServiceHandle(service);
  CloseServiceHandle(manager);

  return started;
}

bool Service::Uninstall(const std::string& serviceName)
{
  auto manager = OpenManager(SC_MANAGER_CONNECT);
  if (manager == nullptr) {
    return false;
  }

  auto service = OpenService(manager,
                             serviceName.c_str(),
                             STANDARD_RIGHTS_REQUIRED | SERVICE_STOP |
                                 SERVICE_QUERY_STATUS);
  if (service == nullptr) {
    printf("OpenService failed (%lu)\n", GetLastError());
    CloseServiceHandle(manager);
    return false;
  }

  if (!Stop(service)) {
    return false;
  }

  if (DeleteService(service) == 0) {
    printf("DeleteService failed (%lu)\n", GetLastError());
    CloseServiceHandle(service);
    CloseServiceHandle(manager);
    return false;
  }

  CloseServiceHandle(service);
  CloseServiceHandle(manager);
  return true;
}

bool Service::IsInstalled(const std::string& serviceName)
{
  auto manager = OpenManager(SC_MANAGER_CONNECT);
  if (manager == nullptr) {
    return false;
  }

  auto service =
      OpenService(manager, serviceName.c_str(), SERVICE_QUERY_STATUS);
  if (service == nullptr) {
    CloseServiceHandle(manager);
    return false;
  }

  CloseServiceHandle(service);
  CloseServiceHandle(manager);
  return true;
}

bool Service::Start(const std::string& serviceName)
{
  auto manager = OpenManager(SC_MANAGER_CONNECT);
  if (manager == nullptr) {
    return false;
  }

  auto service = OpenService(
      manager, serviceName.c_str(), SERVICE_QUERY_STATUS | SERVICE_START);
  if (service == nullptr) {
    printf("OpenService failed (%lu)\n", GetLastError());
    CloseServiceHandle(manager);
    return false;
  }

  bool started = Start(service);

  CloseServiceHandle(service);
  CloseServiceHandle(manager);

  return started;
}

bool Service::Stop(const std::string& serviceName)
{
  auto manager = OpenManager(SC_MANAGER_CONNECT);
  if (manager == nullptr) {
    return false;
  }

  auto service = OpenService(
      manager, serviceName.c_str(), SERVICE_QUERY_STATUS | SERVICE_STOP);
  if (service == nullptr) {
    printf("OpenServiceA failed (%lu)\n", GetLastError());
    CloseServiceHandle(manager);
    return false;
  }

  bool stopped = Stop(service);

  CloseServiceHandle(service);
  CloseServiceHandle(manager);

  return stopped;
}

bool Service::Start(SC_HANDLE service)
{
  SERVICE_STATUS status;
  StartService(service, 0, nullptr);
  return WaitForStatus(service, SERVICE_START_PENDING, SERVICE_RUNNING);
}

bool Service::Stop(SC_HANDLE service)
{
  SERVICE_STATUS status;
  ControlService(service, SERVICE_CONTROL_STOP, &status);
  return WaitForStatus(service, SERVICE_STOP_PENDING, SERVICE_STOPPED);
}

DWORD Service::GetStatus(SC_HANDLE service)
{
  SERVICE_STATUS status;
  if (!QueryServiceStatus(service, &status)) {
    return -1;
  }
  return status.dwCurrentState;
}

bool Service::WaitForStatus(SC_HANDLE service,
                            DWORD waitStatus,
                            DWORD desiredStatus)
{
  SERVICE_STATUS_PROCESS status;
  DWORD startTime = GetTickCount();
  DWORD waitTime;
  DWORD timeout = 30000; // 30-second timeout
  DWORD bytesNeeded;

  // Check if the service is already in the desired status.
  if (!QueryServiceStatusEx(service,
                            SC_STATUS_PROCESS_INFO,
                            (LPBYTE)&status,
                            sizeof(SERVICE_STATUS_PROCESS),
                            &bytesNeeded)) {
    printf("QueryServiceStatusEx failed (%lu)\n", GetLastError());
    return false;
  }

  if (desiredStatus == status.dwCurrentState) {
    return true;
  }

  // Wait for the service to reach the desired status.
  while (status.dwCurrentState == waitStatus) {
    // Don't wait longer than the wait hint. A good interval is 1/10th of
    // the wait hint but >=1 second and <=10 seconds.
    waitTime = status.dwWaitHint / 10;
    if (waitTime < 1000) {
      waitTime = 1000;
    } else if (waitTime > 10000) {
      waitTime = 10000;
    }

    Sleep(waitTime);

    // Check if the service reached the desired status. If it did, return true
    // immediately. Otherwise, continue waiting until the timeout.
    if (!QueryServiceStatusEx(service,
                              SC_STATUS_PROCESS_INFO,
                              (LPBYTE)&status,
                              sizeof(SERVICE_STATUS_PROCESS),
                              &bytesNeeded)) {
      printf("QueryServiceStatusEx failed (%lu)\n", GetLastError());
      return false;
    }

    if (status.dwCurrentState == desiredStatus) {
      return true;
    }

    if (GetTickCount() - startTime > timeout) {
      return false;
    }
  }

  return false;
}

SC_HANDLE Service::OpenManager(DWORD rights)
{
  auto handle = OpenSCManager(nullptr, nullptr, rights);
  if (handle != nullptr) {
    return handle;
  }

  printf("OpenSCManager failed (%lu)\n", GetLastError());
  return nullptr;
}
