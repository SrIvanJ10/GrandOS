#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <microhttpd.h>

#define PORT 8080
#define HTML_FILE "index.html"

static enum MHD_Result answer_to_connection(void *cls,
                                            struct MHD_Connection *connection,
                                            const char *url,
                                            const char *method,
                                            const char *version,
                                            const char *upload_data,
                                            size_t *upload_data_size,
                                            void **con_cls)
{
    (void)cls;
    (void)url;
    (void)method;
    (void)version;
    (void)upload_data;
    (void)upload_data_size;
    (void)con_cls;

    /* Read the HTML file */
    FILE *f = fopen(HTML_FILE, "rb");
    if (!f) {
        const char *error_page = "<html><body><h1>404 - index.html not found</h1></body></html>";
        struct MHD_Response *response = MHD_create_response_from_buffer(
            strlen(error_page), (void *)error_page, MHD_RESPMEM_PERSISTENT);
        MHD_add_response_header(response, "Content-Type", "text/html");
        enum MHD_Result ret = MHD_queue_response(connection, MHD_HTTP_NOT_FOUND, response);
        MHD_destroy_response(response);
        return ret;
    }

    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *page = malloc(fsize + 1);
    if (!page) {
        fclose(f);
        return MHD_NO;
    }
    fread(page, 1, fsize, f);
    fclose(f);
    page[fsize] = '\0';

    struct MHD_Response *response = MHD_create_response_from_buffer(
        fsize, page, MHD_RESPMEM_MUST_FREE);
    MHD_add_response_header(response, "Content-Type", "text/html; charset=utf-8");

    enum MHD_Result ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
    MHD_destroy_response(response);
    return ret;
}

int main(void)
{
    struct MHD_Daemon *daemon = MHD_start_daemon(
        MHD_USE_INTERNAL_POLLING_THREAD,
        PORT,
        NULL, NULL,
        &answer_to_connection, NULL,
        MHD_OPTION_END);

    if (!daemon) {
        fprintf(stderr, "Error: no se pudo iniciar el servidor en el puerto %d\n", PORT);
        return EXIT_FAILURE;
    }

    printf("Servidor corriendo en http://localhost:%d\n", PORT);
    printf("Presiona ENTER para detener...\n");
    getchar();

    MHD_stop_daemon(daemon);
    return EXIT_SUCCESS;
}
