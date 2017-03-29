// Malware scanner starter code.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <linux/limits.h>
#include "yara.h"

void showUsage( const char* prog )
{
    printf( "Usage: %s [opts...] [dir]\n", prog );
}

void scanProcs( YR_RULES* rules )
{
    // TODO: scan processes
}

void scanDir( const char* szDir, YR_RULES* rules )
{
    // TODO: scan dir
}

void cbCompile( int errLevel,
                const char* szFile,
                int lineNum,
                const char* szMsg,
                void* uData )
{
    // TODO: fill me
}

int initYara( const char* szRulePath, YR_RULES** rules )
{
    int r;
    FILE* fp;
    YR_COMPILER* compiler;

    if ( yr_initialize() != ERROR_SUCCESS ) {
        fprintf( stderr, "Failed to initialize YARA\n" );
        return -1;
    }

    if ( yr_compiler_create( &compiler ) != ERROR_SUCCESS ) {
        fprintf( stderr, "Failed to initialize YARA\n" );
        yr_finalize();
        return -1;
    }

    yr_compiler_set_callback( compiler, cbCompile, NULL );

    fp = fopen( szRulePath, "r" );
    if ( !fp ) {
        fprintf( stderr, "Could not open the rule file.\n" );
        yr_finalize();
        return -1;
    }

    r = yr_compiler_add_file( compiler, fp, NULL, szRulePath );

    fclose( fp );

    if ( r ) { // If there exists an error.
        fprintf( stderr, "Compile error.\n" );
        yr_finalize();
        return -1;
    }

    yr_compiler_get_rules( compiler, rules );
    return 0;
}


int main( int argc, char* argv[] )
{
    int opt;
    YR_RULES* rules = NULL;
    char* szRulePath = NULL;

    while ( ( opt = getopt( argc, argv, "hr:" ) ) != -1 ) {
        switch ( opt ) {
        case 'r': // rule file
            if ( szRulePath ) {
                fprintf( stderr, "Cannot specify multiple rule files.\n" );
                free( szRulePath );
                exit( EXIT_FAILURE );
            }
            szRulePath = strndup( optarg, PATH_MAX );
            break;
        case 'h': // help
        default:
            showUsage( argv[0] );
            exit( EXIT_FAILURE );
        }
    }

    if ( szRulePath == NULL ) {
        fprintf( stderr, "Rule file is not given.\n" );
        return EXIT_FAILURE;
    } else {
        initYara( szRulePath, &rules );
    }

    if ( optind >= argc ) scanProcs( rules );

    while ( optind < argc ) {
        scanDir( argv[optind++], rules );
    }

    yr_finalize();
    free( szRulePath );
    return EXIT_SUCCESS;
}
