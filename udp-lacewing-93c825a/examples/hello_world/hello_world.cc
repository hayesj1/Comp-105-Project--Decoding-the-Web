
/* See `hello_world.c` for a C version */

#include "lacewing.h"
using namespace lacewing;

void on_get (webserver webserver, webserver_request request)
{
    request->writef ("Hello world from %s", lw_version ());
}

int main ()
{
    eventpump eventpump = eventpump_new ();
    webserver webserver = webserver_new (eventpump);

    webserver->on_get (on_get);

    webserver->host (8080);    
    
    eventpump->start_eventloop();

    webserver_delete (webserver);
    pump_delete (eventpump);
    
    return 0;
}

