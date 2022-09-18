#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <string>

class Service
{
public:
  /**
   * Install and start a Windows service.
   * @param serviceName Service name
   * @param displayName Service display name
   * @param fileName Path to the service executable
   * @return True if the service was installed and started successfully, false
   * otherwise.
   */
  static bool Install(const std::string& serviceName,
                      const std::string& displayName,
                      const std::string& fileName);

  /**
   * Stop and uninstall a Windows service.
   * @param serviceName Service name
   * @return True if the service was stopped and uninstalled successfully, false
   * otherwise.
   */
  static bool Uninstall(const std::string& serviceName);

  /**
   * Check if a Windows service with the given name is installed.
   * @param serviceName Service name
   * @return True if the service is installed, false otherwise.
   */
  static bool IsInstalled(const std::string& serviceName);

  /**
   * Start a Windows service.
   * @param serviceName Service name
   * @return True if the service was started successfully, false otherwise.
   */
  static bool Start(const std::string& serviceName);

  /**
   * Stop a Windows service.
   * @param serviceName Service name
   * @return True if the service was stopped successfully, false otherwise.
   */
  static bool Stop(const std::string& serviceName);

private:
  static bool Start(SC_HANDLE service);

  static bool Stop(SC_HANDLE service);

  static DWORD GetStatus(SC_HANDLE service);

  static bool WaitForStatus(SC_HANDLE service,
                            DWORD waitStatus,
                            DWORD desiredStatus);

  static SC_HANDLE OpenManager(DWORD rights);
};