double setDefaultDouble(char *envName, double dflt)
/*
 * If there's an environment variable named `envName`, convert it to a
 * double and return it. Otherwise, return the default value `dflt`.
 */
{
    char *env = getenv(envName);

    return ( env ? strtod(env, NULL) : dflt );
}