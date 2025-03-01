#include "kv.h"


static inline bool kv_parse_bool(void *dataptr, char *data)
{
    if (strcmp(data, "true") == 0 || 
        strcmp(data, "yes") == 0 ||
        strcmp(data, "on") == 0)
    {
        *(bool*)dataptr = true;
        return true;
    }
    else if (strcmp(data, "false") == 0 || 
            strcmp(data, "no") == 0 ||
            strcmp(data, "off") == 0)
    {
        *(bool*)dataptr = false;
        return false;
    }

    ZRUB_LOG_ERROR("received unexpected value `%s` as bool\n", data);
    return false;
}

static inline bool kv_parse_u32(void *dataptr, char *data)
{
    char *ptr;
    u32 val = strtoul(data, &ptr, 10); 

    // checks for invalid characters
    if (*ptr != '\0' && !isspace((unsigned char)*ptr)) 
    {
        return false;
    }
    if (val >= UINT32_MAX)
    {
        ZRUB_LOG_ERROR("value is probably too large\n");
    }

    *(u32*)dataptr = (u32)val;
    return true;
}

/**
 * @brief this method infers data based on the second field
 * 
 * @param dataptr       pointer to the data
 * @param data          the data from the config file line
 * @returns true if successfully inferred the data according to the second field
 */
static bool kv_infer_data(void *dataptr, char *data)
{
    char key[MAXSECT];
    char type[MAXSECT];
    char value[MAXSECT];

    if (sscanf(data, "%s %s %[^\n]", key, type, value) == 3) {
        ZRUB_LOG_DEBUG("(key=%s) (type=%s) (value=%s)\n", key, type, value);
    }
    else
    {
        ZRUB_LOG_ERROR("failed to get key, type or value\n");
        return false;
    }

    if (strcmp(type, "str") == 0)
    {
        snprintf(dataptr, strlen(value) + 1, "%s", value);
        ZRUB_LOG_DEBUG("set as `%s`\n", (char*)dataptr);

        return true;
    }

    else if (strcmp(type, "bool") == 0)
    {
        return kv_parse_bool(dataptr, value);
    }

    else if (strcmp(type, "u32") == 0)
    {
        return kv_parse_u32(dataptr, value);
    }

    return false;
}

/**
 * @brief this method retreives the data from an ORA config file to a
 * key entered
 * 
 * @example raw_config is `user.name    str     orpheus` and dataptr is
 * some char*, then num_args=2 and the other args are "user" and "name"
 * where the order matters to get user.name key.
 *
 * 
 * @param raw_config        content of an ORA config file
 * @param dataptr           pointer to a datatype
 * @param num_args          count of the va_args
 * @returns true if managed to find the key AND retrieve its value to dataptr
 */
bool zrub_get_kv_cfg(char *raw_config, void *dataptr, i32 num_args, ...)
{
    va_list args;
    va_start(args, num_args);

    char opt[MAXLINE] = "\0";
    char *tmp;
    char *dot = ".";

    for (i32 i = 0; i < num_args; i++)
    {
        tmp = va_arg(args, char*);
        strncat(opt, tmp, MAXLINE - strlen(tmp) - 1);

        if (i < num_args - 1)
        {
            strncat(opt, dot, MAXLINE - strlen(dot) - 1);
        }
    }

    va_end(args);
    ZRUB_LOG_DEBUG("%s\n", opt);

    char line[MAXLINE];
    char *start = raw_config;
    char *end;
    u8 counter = 0;

    while (*start)
    {
        end = strchr(start, '\n');

        if (end)
        {
            u16 len = end - start;

            // to avoid buffer overflows
            if (len >= MAXLINE)
            {
                len = MAXLINE - 1;
            }

            strncpy(line, start, len);  
            line[len] = '\0';
          
            if (strncmp(line, opt, strlen(opt)) == 0)
            {
                ZRUB_LOG_DEBUG("found equal `%s`\n", line);
                return kv_infer_data(dataptr, line);
            }
            
            start = end + 1;
        }
        else
        {
            strncpy(line, start, MAXLINE - 1);
            line[MAXLINE - 1] = '\0';

            if (strncmp(line, opt, strlen(opt)) == 0)
            {
                ZRUB_LOG_DEBUG("found equal `%s`\n", line);
                return kv_infer_data(dataptr, line);
            }

            break;
        }
        
        counter += 1;
    }
    
    ZRUB_LOG_DEBUG("no such cfg option\n");
    return false;
}
