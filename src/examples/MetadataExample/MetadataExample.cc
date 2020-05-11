

#include <JANA/JApplication.h>
#include <JANA/JFactoryGenerator.h>

#include "MetadataAggregator.h"

extern "C" {
void InitPlugin(JApplication* app) {

    // This code is executed when the plugin is attached.
    // It should always call InitJANAPlugin(app) first, and then do one or more of:
    //   - Read configuration parameters
    //   - Register JFactoryGenerators
    //   - Register JEventProcessors
    //   - Register JEventSourceGenerators (or JEventSources directly)
    //   - Register JServices

    InitJANAPlugin(app);

    LOG << "Loading MetadataExample" << LOG_END;
    app->Add(new MetadataExampleProcessor);
    // Add any additional components as needed
}
}

