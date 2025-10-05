#ifndef SYSTEMTIMEIMPL_HPP_
#define SYSTEMTIMEIMPL_HPP_

#include <Falco/SystemTime/SystemTimeComponentAc.hpp>

namespace Falco {

class SystemTime: public SystemTimeComponentBase {
    public:
        SystemTime(const char* compName);
        virtual ~SystemTime();
        void init(NATIVE_INT_TYPE instance);
    protected:
      //! Handler implementation for timeGetPort
      //!
      //! Port to retrieve time
      void timeGetPort_handler(
        NATIVE_INT_TYPE portNum, //!< The port number
        Fw::Time& time //!< The U32 cmd argument
      ) override;
      
    private:
};

}

#endif /* SYSTEMTIMEIMPL_HPP_ */
