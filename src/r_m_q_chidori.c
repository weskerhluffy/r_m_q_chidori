/*
 ============================================================================
 Name        : r_m_q_chidori.c
 Author      : ernesto
 Version     :
 Copyright   : a veces siento que
 Description : Hello World in C, Ansi-style
 ============================================================================
 XXX: http://www.spoj.com/problems/RMQSQ/
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>
#include <stddef.h>
#include <unistd.h>
#include <sys/time.h>
#include <math.h>

#ifdef CACA_COMUN_LOG
#include <execinfo.h>
#endif

#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#else
#include <time.h>
#include <stdarg.h>
#endif

#define CACA_COMUN_TAM_MAX_LINEA (8*30000+30000)
#define CACA_LOG_MAX_TAM_CADENA 2000

#define CACA_COMUN_BUF_STATICO (char[1000] ) { '\0' }

#define BITCH_VECTOR_NUM_BITS (sizeof(bitch_vector) * 8)

#define CACA_COMUN_ASSERT_DUROTE 0
#define CACA_COMUN_ASSERT_SUAVECITO 1
#define CACA_COMUN_ASSERT_NIMADRES 2

typedef unsigned int natural;
typedef int tipo_dato;

typedef long long bitch_vector;

typedef enum BOOLEANOS {
	falso = 0, verdadero
} bool;

#define CACA_COMUN_TIPO_ASSERT CACA_COMUN_ASSERT_SUAVECITO
/*
 #define CACA_COMUN_TIPO_ASSERT CACA_COMUN_ASSERT_DUROTE
 */

#define assert_timeout_dummy(condition) 0;
//static inline void caca_log_debug_dummy(const char *format, ...) { }

#if CACA_COMUN_TIPO_ASSERT == CACA_COMUN_ASSERT_DUROTE
#define assert_timeout(condition) assert(condition);
#endif
#if CACA_COMUN_TIPO_ASSERT == CACA_COMUN_ASSERT_SUAVECITO
#define assert_timeout(condition) if(!(condition)){sleep(10);abort();}
#endif
#if CACA_COMUN_TIPO_ASSERT == CACA_COMUN_ASSERT_NIMADRES
#define assert_timeout(condition) 0
#endif

#ifdef CACA_COMUN_LOG
#define caca_log_debug(formato, args...) \
		do \
		{ \
			size_t profundidad = 0; \
			void *array[CACA_LOG_MAX_TAM_CADENA]; \
 			profundidad = backtrace(array, CACA_LOG_MAX_TAM_CADENA); \
			caca_log_debug_func(formato,__FILE__, __func__, __LINE__,profundidad,##args); \
		} \
		while(0);
#else
#define caca_log_debug(formato, args...) 0;
//#define caca_log_debug caca_log_debug_dummy
#endif

#define caca_comun_max(x,y) ((x) < (y) ? (y) : (x))
#define caca_comun_min(x,y) ((x) < (y) ? (x) : (y))

void caca_log_debug_func(const char *format, ...);

#define caca_comun_mapa_bitch_checa(bits, posicion, resultado) \
        __asm__ (\
                        "xor %%rdx,%%rdx\n"\
                        "movq %[bitch_posi],%%rax\n" \
                        "movq $64,%%r8\n"\
                        "divq %%r8\n"\
                        "mov $1,%[resul]\n"\
                        "mov %%rdx,%%rcx\n"\
                        "shlq %%cl,%[resul]\n"\
                        "and (%[vectors],%%rax,8),%[resul]\n"\
                        : [resul] "=b" (resultado)\
                        : [bitch_posi] "r" (posicion), [vectors] "r" (bits)\
            :"rax","rdx","rcx","r8")

static inline void caca_comun_mapa_bitch_asigna(bitch_vector *bits,
		unsigned long posicion) {
	int idx_arreglo = 0;
	int idx_registro = 0;

	idx_arreglo = posicion / 64;
	idx_registro = posicion % 64;

	bits[idx_arreglo] |= (bitch_vector) ((bitch_vector) 1 << idx_registro);

}

static inline void caca_comun_mapa_bitch_limpia(bitch_vector *bits,
		unsigned long posicion) {
	int idx_arreglo = 0;
	int idx_registro = 0;

	idx_arreglo = posicion / 64;
	idx_registro = posicion % 64;

	bits[idx_arreglo] &= (bitch_vector) ~((bitch_vector) 1 << idx_registro);

}

void caca_comun_current_utc_time(struct timespec *ts) {

#ifdef __MACH__
	clock_serv_t cclock;
	mach_timespec_t mts;
	host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
	clock_get_time(cclock, &mts);
	mach_port_deallocate(mach_task_self(), cclock);
	ts->tv_sec = mts.tv_sec;
	ts->tv_nsec = mts.tv_nsec;
#else
#ifdef CACA_COMUN_LOG
	clock_gettime(CLOCK_REALTIME, ts);
#endif
#endif

}

void caca_comun_timestamp(char *stime) {
	time_t ltime;
	struct tm result;
	long ms;
	struct timespec spec;
	char parte_milisecundos[50];

	ltime = time(NULL );

#ifndef ONLINE_JUDGE
	localtime_r(&ltime, &result);
	asctime_r(&result, stime);
#endif

	*(stime + strlen(stime) - 1) = ' ';

	caca_comun_current_utc_time(&spec);
#ifndef ONLINE_JUDGE
	ms = round(spec.tv_nsec / 1.0e3);
#endif
	sprintf(parte_milisecundos, "%ld", ms);
	strcat(stime, parte_milisecundos);
}
void caca_log_debug_func(const char *format, ...) {

	va_list arg;
	va_list arg2;
	const char *PEDAZO_TIMESTAMP_HEADER = "tiempo: %s; ";
	const char *HEADER =
			"archivo: %s; funcion: %s; linea %d; nivel: %zd caca 8====D ";
	char formato[CACA_LOG_MAX_TAM_CADENA + sizeof(HEADER)
			+ sizeof(PEDAZO_TIMESTAMP_HEADER)] = { '\0' };
	char pedazo_timestamp[sizeof(PEDAZO_TIMESTAMP_HEADER) + 100] = { '\0' };
	char cadena_timestamp[100] = { '\0' };

	caca_comun_timestamp(cadena_timestamp);
	sprintf(pedazo_timestamp, PEDAZO_TIMESTAMP_HEADER, cadena_timestamp);

	strcpy(formato, pedazo_timestamp);
	strcat(formato, HEADER);
	strcat(formato, format);
	strcat(formato, "\n");

	va_start(arg, format);
	va_copy(arg2, arg);
	vprintf(formato, arg2);
	va_end(arg2);
	va_end(arg);
// XXX: http://stackoverflow.com/questions/1716296/why-does-printf-not-flush-after-the-call-unless-a-newline-is-in-the-format-strin
	setbuf(stdout, NULL );
}

#ifdef CACA_COMUN_LOG
static char *caca_comun_arreglo_a_cadena(tipo_dato *arreglo, int tam_arreglo,
		char *buffer) {
	int i;
	char *ap_buffer = NULL;
	int characteres_escritos = 0;
#ifdef ONLINE_JUDGE
	return NULL;
#endif

	memset(buffer, 0, 100);
	ap_buffer = buffer;

	for (i = 0; i < tam_arreglo; i++) {
		characteres_escritos += sprintf(ap_buffer + characteres_escritos,
				"%u", *(arreglo + i));
		if (i < tam_arreglo - 1) {
			*(ap_buffer + characteres_escritos++) = ',';
		}
	}
	*(ap_buffer + characteres_escritos) = '\0';
	return ap_buffer;
}

static char *caca_comun_arreglo_a_cadena_natural(natural *arreglo,
		natural tam_arreglo, char *buffer) {
	int i;
	char *ap_buffer = NULL;
	int characteres_escritos = 0;
#ifdef ONLINE_JUDGE
	return NULL;
#endif

	memset(buffer, 0, 100);
	ap_buffer = buffer;

	for (i = 0; i < tam_arreglo; i++) {
		characteres_escritos += sprintf(ap_buffer + characteres_escritos, "%2u",
				*(arreglo + i));
		if (i < tam_arreglo - 1) {
			*(ap_buffer + characteres_escritos++) = ',';
		}
	}
	*(ap_buffer + characteres_escritos) = '\0';
	return ap_buffer;
}
#endif
void caca_comun_strreplace(char s[], char chr, char repl_chr) {
	int i = 0;
	while (s[i] != '\0') {
		if (s[i] == chr) {
			s[i] = repl_chr;
		}
		i++;
	}
}

static inline int caca_comun_lee_matrix_long_stdin(tipo_dato *matrix,
		int *num_filas, int *num_columnas, int num_max_filas,
		int num_max_columnas) {
	int indice_filas = 0;
	int indice_columnas = 0;
	tipo_dato numero = 0;
	char *siguiente_cadena_numero = NULL;
	char *cadena_numero_actual = NULL;
	char *linea = NULL;

	linea = calloc(CACA_COMUN_TAM_MAX_LINEA, sizeof(char));

	while (indice_filas < num_max_filas
			&& fgets(linea, CACA_COMUN_TAM_MAX_LINEA, stdin)) {
		indice_columnas = 0;
		cadena_numero_actual = linea;
		caca_comun_strreplace(linea, '\n', '\0');
		if (!strlen(linea)) {
			caca_log_debug("weird, linea vacia");
			continue;
		}
		for (siguiente_cadena_numero = linea;; siguiente_cadena_numero =
				cadena_numero_actual) {
//			caca_log_debug("el numero raw %s", linea);
			numero = strtol(siguiente_cadena_numero, &cadena_numero_actual, 10);
			if (cadena_numero_actual == siguiente_cadena_numero) {
				break;
			}
			*(matrix + indice_filas * num_max_columnas + indice_columnas) =
					numero;
//			caca_log_debug("en col %d, fil %d, el valor %lu", indice_columnas,
//					indice_filas, numero);
			indice_columnas++;
//			caca_log_debug("las columnas son %d", indice_columnas);
		}
		if (num_columnas) {
			num_columnas[indice_filas] = indice_columnas;
		}
		indice_filas++;
		caca_log_debug("las filas son %d, con clos %d", indice_filas,
				indice_columnas);
	}

	*num_filas = indice_filas;
	free(linea);
	return 0;
}

#define R_M_Q_CHIDORI_MAX_NUMEROS ((natural)(1E5))

#if 1
#define TROZO_TREE_MAX_NUMEROS R_M_Q_CHIDORI_MAX_NUMEROS
#define TROZO_TREE_MAX_NODOS ((TROZO_TREE_MAX_NUMEROS<<1)-1)
#define TROZO_TREE_MAX_VALOR INT_MAX

typedef struct trozo_tree {
	struct trozo_tree *hijo_izq;
	struct trozo_tree *hijo_der;
	tipo_dato valor;
} trozo_tree;

trozo_tree nodos_trozo[TROZO_TREE_MAX_NODOS] = { 0 };
natural nodos_trozo_usados = 0;
tipo_dato trozo_tree_numeros[TROZO_TREE_MAX_NUMEROS] = { 0 };
natural trozo_tree_numeros_tam = 0;
natural idx_ini_buscado = 0;
natural idx_fin_buscado = 0;

static inline trozo_tree *trozo_tree_nuevo_nodo() {
	trozo_tree *nodo_nuevo = (nodos_trozo + (nodos_trozo_usados++));
	return nodo_nuevo;
}

static inline void trozo_tree_contruye(trozo_tree **nodo_actual,
		natural idx_ini, natural idx_fin) {
	trozo_tree *nodo_actual_int = NULL;
	if (!*nodo_actual) {
		*nodo_actual = trozo_tree_nuevo_nodo();
	}
	nodo_actual_int = *nodo_actual;
	if (idx_ini != idx_fin) {
		natural idx_mid = idx_ini + ((idx_fin - idx_ini) >> 1);

		trozo_tree_contruye(&nodo_actual_int->hijo_izq, idx_ini, idx_mid);
		trozo_tree_contruye(&nodo_actual_int->hijo_der, idx_mid + 1, idx_fin);

		nodo_actual_int->valor =
				caca_comun_min(nodo_actual_int->hijo_izq->valor,nodo_actual_int->hijo_der->valor);
	} else {
		nodo_actual_int->valor = trozo_tree_numeros[idx_ini];
	}
}

static inline tipo_dato trozo_tree_consulta(trozo_tree *nodo_actual,
		natural idx_ini, natural idx_fin) {
	tipo_dato resul = TROZO_TREE_MAX_VALOR;
	if (idx_ini_buscado > idx_fin) {
		resul = TROZO_TREE_MAX_VALOR;
	} else {
		if (idx_fin_buscado < idx_ini) {
			resul = TROZO_TREE_MAX_VALOR;
		} else {
			if (idx_ini == idx_ini_buscado && idx_fin == idx_fin_buscado) {
				resul = nodo_actual->valor;
			} else {
				natural idx_mid = idx_ini + ((idx_fin - idx_ini) >> 1);
				tipo_dato resul_izq = 0;
				tipo_dato resul_der = 0;

				resul_izq = trozo_tree_consulta(nodo_actual->hijo_izq, idx_ini,
						idx_mid);
				resul_der = trozo_tree_consulta(nodo_actual->hijo_der,
						idx_mid + 1, idx_fin);

				resul = caca_comun_min(resul_izq,resul_der);
			}
		}
	}
	return resul;
}

#endif

void r_m_q_chidori_core(natural *consultas, natural consultas_tam) {
	trozo_tree *raiz = NULL;

	trozo_tree_contruye(&raiz, 0, trozo_tree_numeros_tam);

	for (int i = 0; i < consultas_tam; i++) {
		tipo_dato res = 0;
		idx_ini_buscado = *(consultas + i * 2);
		idx_fin_buscado = *(consultas + i * 2 + 1);
		caca_log_debug("el idx ini busq %u el idx fin busq %u", idx_ini_buscado,
				idx_fin_buscado);

		res = trozo_tree_consulta(raiz, 0, trozo_tree_numeros_tam);

	}

}

natural consultas[100000][2] = { 0 };

void r_m_q_chidori_main() {
	natural consultas_tam = 0;

	scanf("%u", &trozo_tree_numeros_tam);

	caca_comun_lee_matrix_long_stdin(trozo_tree_numeros, &(int ) { 0 }, NULL, 1,
			trozo_tree_numeros_tam);

	scanf("%u", &consultas_tam);

	caca_comun_lee_matrix_long_stdin((tipo_dato *)consultas, &(int ) { 0 }, NULL,
			consultas_tam, 2);

	r_m_q_chidori_core((natural *)consultas, consultas_tam);
}

int main(void) {
	r_m_q_chidori_main();
	return EXIT_SUCCESS;
}
