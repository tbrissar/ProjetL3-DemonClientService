#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"

static void usage(const char *exeName, const char *message)
{
    fprintf(stderr, "usage : %s <fichier config>\n", exeName);
    if (message != NULL)
        fprintf(stderr, "message : %s\n", message);
    exit(EXIT_FAILURE);
}

void test_config(const char *filename)
{
    int nb;

    config_init(filename);
    nb = config_getNombre();
    printf("Il y a %d service(s)\n", nb);
    for (int i = 1; i <= nb; i++){
        printf("service %d : %s (%s)\n",
               i,
               (config_isOuvert(i) ? "ouvert" : "ferme "),
               config_getNomExecutable(i));
    }
    config_exit();
}

int main(int argc, char * argv[])
{
    if (argc != 2)
        usage(argv[0], NULL);

    test_config(argv[1]);

    return EXIT_SUCCESS;
}
