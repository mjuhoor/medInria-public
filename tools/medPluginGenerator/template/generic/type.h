// /////////////////////////////////////////////////////////////////
// Generated by dtkPluginGenerator
// /////////////////////////////////////////////////////////////////

#ifndef %2_H
#define %2_H

#include <dtkCore/dtkAbstract%3.h>

#include "%1PluginExport.h"

namespace %5
{
    
    class %1Private;
    
    class %2PLUGIN_EXPORT %1 : public dtkAbstract%3
    {
        Q_OBJECT
        
    public:
        %1(void);
        virtual ~%1(void);
        
        virtual QString description(void) const;
        
        static bool registered(void);
        
    private:
        %1Private *d;
    };
    
    dtkAbstract%3 *create%4(void);
    
} // end namespace %5

#endif
