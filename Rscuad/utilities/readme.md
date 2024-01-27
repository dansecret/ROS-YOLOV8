# Utilities
> all packet lib not specific RSCUAD

## how to use?
#### print

        RSCUAD_PRINT("Hello world");            // normal font
        RSCUAD_ERROR("Hello world");            // red font
        RSCUAD_WARNING("Hello world");          // yellow font
        RSCUAD_INFO("Hello world");             // blue font
        RSCUAD_SUCCESS("Hello world");          // green font

        RSCUAD_PRINT(_RED "Hello world");       // manual font

#### swap

        Swap::str("hello"); // this is static