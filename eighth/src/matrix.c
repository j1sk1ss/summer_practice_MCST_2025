#include "../include/matrix.h"


volatile sig_atomic_t _sigint_received = 0;
volatile sig_atomic_t _sigint_count    = 0;

volatile int _current_i = -1;
volatile int _current_j = -1;


/*
Using write instead printf. Write is acync_safe for signals. Printf not.
Also another formating functions like sprintf unsafe too, that's why we should
print numbers with only one allowed safe function write.
*/
static int _int2str(int num, char* buff) {
    char temp[12] = { 0 };
    int pos = 0, len = 0;
    if (num == 0) {
        buff[0] = '0';
        return 1;
    }
    
    if (num < 0) {
        buff[len++] = '-';
        num = -num;
    }

    while (num > 0) {
        temp[pos++] = '0' + (num % 10);
        num /= 10;
    }

    for (int i = pos - 1; i >= 0; --i) buff[len++] = temp[i];
    return len;
}

static void _sigint_handler(int signo) {
    char message[128] = { 0 };

    const char* fmsg = "\n[SIGINT] Caught SIGINT. Current indices: i=";
    const char* smsg = ", j=";
    const char* tsmg = "\n";

    int pos = 0;
    for (int i = 0; fmsg[i]; ++i) message[pos++] = fmsg[i];
    pos += _int2str(_current_i, &message[pos]);
    for (int i = 0; smsg[i]; ++i) message[pos++] = smsg[i];
    pos += _int2str(_current_j, &message[pos]);
    for (int i = 0; tsmg[i]; ++i) message[pos++] = tsmg[i];

    write(STDOUT_FILENO, message, sizeof(message) - 1);
    if (_sigint_count++ == 1)  _exit(EXIT_FAILURE);
}

/*
Return 1 if success setup.
Return 0 if something goes wrong.
*/
int setup_sigaction() {
    struct sigaction sa;
    sa.sa_handler = _sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    return !sigaction(SIGINT, &sa, NULL);
}

/*
Return 1 if success setup.
Return 0 if something goes wrong.
*/
int setup_signal() {
    return signal(SIGINT, _sigint_handler) != SIG_ERR;
}

int new_matrix(matrix_t* mtrx, int x, int y) {
    unsigned int* matrix_raw = (unsigned int*)malloc(x * y * sizeof(unsigned int));
    if (!matrix_raw) return 0;

    for (int i = 0; i < x * y; i++)matrix_raw[i] = 0;
    mtrx->body = matrix_raw;
    mtrx->x = x;
    mtrx->y = y;

    return 1;
}

int print_matrix(matrix_t* matrix, char* method, FILE* fd) {
    for (int i = 0; i < matrix->x; i++) {
        for (int j = 0; j < matrix->y; j++) fprintf(fd, method, matrix->body[i * matrix->y + j]);
        fprintf(fd, "\n");
    }

    return 1;
}

int fill_matrix_random(matrix_t* matrix, int min, int max) {
    srand(time(0));
    for (int i = 0; i < matrix->x; i++) {
        for (int j = 0; j < matrix->y; j++) {
            matrix->body[i * matrix->y + j] = min + rand() % (max - min + 1);
        }
    }

    return 1;
}

int free_matrix(matrix_t* matrix) {
    free(matrix->body);
    return 1;
}

int matrix_multiply(matrix_t* a, matrix_t* b, matrix_t* out) {
    if (a->y != b->x) return -2;
    if (!new_matrix(out, a->x, b->y)) return -1;
    for (_current_i = 0; _current_i < a->x; _current_i++) {
        for (_current_j = 0; _current_j < b->y; _current_j++) {
            unsigned int sum = 0;
            for (int k = 0; k < a->y; k++) sum += a->body[_current_i * a->y + k] * b->body[k * b->y + _current_j];
            out->body[_current_i * b->y + _current_j] = sum;
            sleep(1);
        }
    }

    return 1;
}