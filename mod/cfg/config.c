#define ZRUBLIB
#include "zrub/zrub.h"

#include <stdio.h>

struct Settings {
    char app_name[255];
    char web_fqdn[255];
    char web_host[255];
    u32 web_port;
};

int main()
{
    // read the config.ora file
    char raw[4096];
    FILE *fptr = fopen("config.ora", "r");

    if (!fptr)
    {
        ZRUB_LOG_ERROR("couldn't open `config.ora`\n");

        return 1;
    }

    fread(raw, 4096, 1, fptr);

    struct Settings st;

    if (!zrub_get_kv_cfg(raw, st.app_name,  2, "app", "name")) { goto cleanup; }
    if (!zrub_get_kv_cfg(raw, st.web_fqdn,  2, "web", "fqdn")) { goto cleanup; }
    if (!zrub_get_kv_cfg(raw, st.web_host,  2, "web", "host")) { goto cleanup; }
    if (!zrub_get_kv_cfg(raw, &st.web_port, 2, "web", "port")) { goto cleanup; }

    char empty[255];
    if (!zrub_get_kv_cfg(raw, empty, 3, "test", "empty", "balls")) { goto cleanup; }

    ZRUB_LOG_INFO("hosting app `%s` on fqdn `%s` on interface %s:%lu\n",
            st.app_name, st.web_fqdn, st.web_host, st.web_port);

cleanup:
    fclose(fptr);

    return 0;
}
