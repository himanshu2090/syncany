// $Id: BytesSentMonitor.cpp 81691 2008-05-14 11:09:21Z johnnyw $

#include "ace/MonitorControl/BytesSentMonitor.h"

#if defined (ACE_HAS_MONITOR_FRAMEWORK) && (ACE_HAS_MONITOR_FRAMEWORK == 1)

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace Monitor_Control
  {
    const char* BytesSentMonitor::default_name_ =
      "OS/Network/BytesSent";

    BytesSentMonitor::BytesSentMonitor (const char* name)
      : Monitor_Base (name)
#if defined (ACE_HAS_WIN32_PDH)
        , WindowsMultiInstanceMonitor (
            ACE_TEXT ("\\Network Interface(*)\\Bytes Sent/sec"))
#elif defined (linux)
        , LinuxNetworkInterfaceMonitor (
            " %*[^:]: %*u %*u %*u %*u %*u %*u %*u %*u %lu %*u")
            /// Scan format for /proc/net/dev
#endif
    {}

    void
    BytesSentMonitor::update (void)
    {
#if defined (ACE_HAS_WIN32_PDH)
      this->win_update ();

      /// Stores value and timestamp with thread-safety.
      this->receive (this->value_.doubleValue);
#elif defined (linux)
      this->lin_update ();

      /// Stores value and timestamp with thread-safety.
      this->receive (static_cast<double> (this->value_));
#endif
    }

    const char*
    BytesSentMonitor::default_name (void)
    {
      return BytesSentMonitor::default_name_;
    }
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

#endif /* ACE_HAS_MONITOR_FRAMEWORK==1 */


