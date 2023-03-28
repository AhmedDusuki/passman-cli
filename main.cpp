#include "argon2.h"
#include <iostream>
#include <iomanip>
#include <getopt.h>
#include <sstream>
#include <string.h>

#define HASHLEN 16
uint8_t hash[HASHLEN];

struct argon2_parameters
{
    uint32_t t_cost,
        m_cost,
        parallelism;
};

argon2_parameters argon2_config{8,
                                (1 << 17),
                                8};

static struct option long_options[] = {
    {"iterations", required_argument, 0, 'i'},
    {"memory", required_argument, 0, 'm'},
    {"parallelism", required_argument, 0, 'p'},
    {0, 0, 0, 0}};

void usage(char *argv0) // print usage to stderr and exit with code 1
{
    std::cerr << "Usage: "
              << argv0
              << " [-i iterations; default 8] "
              << " [-m memory; default 17 (128MB)]" // memory used: 2^mKB
              << " [-p parallelism; default 8]"
              << " example.com"            // website/service
              << " \"john doe\""           // account username
              << " \"master password123\"" // master password
              << " 1"                      // number of password resets
              << std::endl;
    exit(1);
}

int main(int argc, char *argv[])
{
    char c;
    while ((c = getopt_long(argc, argv, "i:m:p:", long_options, NULL)) != -1)
    {
        switch (c)
        {
        case 'i':
        {
            std::istringstream iss(optarg);
            iss >> argon2_config.t_cost;
        }
        break;
        case 'm':
        {
            uint32_t shift_amt;
            std::istringstream iss(optarg);
            iss >> shift_amt;
            argon2_config.m_cost = (1 << shift_amt);
        }
        break;
        case 'p':
        {
            std::istringstream iss(optarg);
            iss >> argon2_config.parallelism;
        }
        break;
        default:
            usage(argv[0]);
        }
    }

    if (optind != argc - 4)
    {
        usage(argv[0]);
    }

    uint32_t pwdlen = strlen(argv[optind + 2]) + 1;
    uint8_t *pwd = new uint8_t[pwdlen];
    memcpy(pwd, argv[optind + 2], pwdlen);

    uint32_t salt_len = strlen(argv[optind]) +
                        strlen(argv[optind + 1]) +
                        strlen(argv[optind + 3]) +
                        1;
    uint8_t *salt = new uint8_t[salt_len];
    memcpy(salt,
           argv[optind],
           strlen(argv[optind]));
    memcpy(salt + strlen(argv[optind]),
           argv[optind + 1],
           strlen(argv[optind + 1]));
    memcpy(salt + strlen(argv[optind]) + strlen(argv[optind + 1]),
           argv[optind + 3],
           strlen(argv[optind + 3]) + 1);

    argon2_context context = {
        hash,         /* output array, at least HASHLEN in size */
        HASHLEN,      /* digest length */
        pwd,          /* password array */
        pwdlen - 1,   /* password length without null byte*/
        salt,         /* salt array */
        salt_len - 1, /* salt length without null byte*/
        NULL, 0,      /* optional secret data */
        NULL, 0,      /* optional associated data */
        argon2_config.t_cost, argon2_config.m_cost, argon2_config.parallelism, argon2_config.parallelism,
        ARGON2_VERSION_13, /* algorithm version */
        NULL, NULL,        /* custom memory allocation / deallocation functions */
        /* by default only internal memory is cleared (pwd is not wiped) */
        ARGON2_FLAG_CLEAR_PASSWORD || ARGON2_FLAG_CLEAR_SECRET};

    int rc = argon2id_ctx(&context);
    if (ARGON2_OK != rc)
    {
        std::cerr << "Error: " << argon2_error_message(rc) << std::endl;
        exit(1);
    }

    memset(salt, 0, salt_len); // clear salt from memory

    std::cout << std::hex << std::setfill('0');
    for (int i = 0; i < HASHLEN; ++i)
        std::cout << std::setw(2) << (int)hash[i];
    std::cout << std::endl;

    memset(hash, 0, HASHLEN); // clear hash from memory

    delete (pwd);
    delete (salt);
    return 0;
}