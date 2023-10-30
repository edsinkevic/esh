//
// Created by edvin on 23.10.30.
//

#include <string.h>
#include <stdlib.h>
#include "esh_utils.h"

char *esh_strdup(char *org) {
    unsigned int org_size;
    static char *dup;
    char *dup_offset;

    /* Allocate memory for duplicate */
    org_size = strlen(org);
    dup = (char *) malloc(sizeof(char) * org_size + 1);
    if (dup == NULL)
        return ((char *) NULL);

    /* Copy string */
    dup_offset = dup;
    while (*org) {
        *dup_offset = *org;
        dup_offset++;
        org++;
    }
    *dup_offset = '\0';

    return (dup);
}