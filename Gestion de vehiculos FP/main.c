#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#ifdef _WIN32
    #include <direct.h>
    #define MKDIR(path) _mkdir(path)
#else
    #include <sys/stat.h>
    #include <unistd.h>
    #define MKDIR(path) mkdir(path, 0700)
#endif

#define MAX_VEHICULOS 200
#define STRLEN 100

#ifdef _WIN32
    #define DATA_DIR_PREFERIDO "C:\\GastosVehiculo"
    #else
      #define DATA_DIR_PREFERIDO "./GastosVehiculo"
#endif

static char g_data_dir[512];
static char g_arch_vehiculos[600];
static char g_arch_precios[600];

typedef struct {
    int id;
    char placa[16];
    char marca[50];
    char modelo[50];
    char tipo_combustible[16];
    float km_por_galon_carretera;
    float km_por_galon_ciudad;
    float costo_gomas;
    float km_gomas;
    float costo_seguro_anual;
    float costo_mantenimiento;
    float km_mantenimiento;
    float costo_vehiculo;
    float vida_util_anios;
    float depreciacion_anual_pct;
    float km_por_anio_promedio;
    int activo;
} Vehiculo;

typedef struct {
    float precio_gasolina;
    float precio_gasoil;
} PreciosCombustible;

typedef struct {
    float costo_gomas;
    float costo_seguro;
    float costo_combustible;
    float costo_mantenimiento;
    float costo_vehiculo;
    float total;
    float costo_por_km;
} ResultadoViaje;

static void leer_linea(char *dest, size_t n) {
    if (fgets(dest, (int)n, stdin)) {
        size_t len = strlen(dest);
        if (len && dest[len-1] == '\n') dest[len-1] = '\0';
    } else {
        if (n) dest[0] = '\0';
        clearerr(stdin);
    }
}

static int leer_int(const char *msg, int minv, int maxv) {
    int v; char line[64];
    for (;;) {
        printf("%s", msg); fflush(stdout);
        leer_linea(line, sizeof(line));
        if (sscanf(line, "%d", &v) == 1 && v >= minv && v <= maxv) return v;
        printf("\nValor invalido. Intente de nuevo.\n\n");
    }
}

static float leer_float(const char *msg, float minv, float maxv) {
    float v; char line[64];
    for (;;) {
        printf("%s", msg); fflush(stdout);
        leer_linea(line, sizeof(line));
        if (sscanf(line, "%f", &v) == 1 && v >= minv && v <= maxv) return v;
        printf("\nValor invalido. Intente de nuevo.\n\n");
    }
}

static void to_lower(char *s) {
    for (; *s; ++s) *s = (char)tolower((unsigned char)*s);
}

static int es_gasoil(const char *tipo) {
    char tmp[32];
    strncpy(tmp, tipo, sizeof(tmp)); tmp[sizeof(tmp)-1] = '\0';
    to_lower(tmp);
    return (strstr(tmp, "gasoil") != NULL) || (strstr(tmp, "diesel") != NULL) || (strstr(tmp, "diésel") != NULL);
}

static void inicializar_directorio_datos(void) {
    strncpy(g_data_dir, DATA_DIR_PREFERIDO, sizeof(g_data_dir));
    g_data_dir[sizeof(g_data_dir)-1] = '\0';

    if (MKDIR(g_data_dir) == 0) {
        printf("\n[Info] Carpeta de datos creada en: %s\n", g_data_dir);
    } else {
        if (errno != EEXIST) {
            fprintf(stderr, "\n[Advertencia] No se pudo crear '%s' (errno=%d). Se usara el directorio actual.\n", g_data_dir, errno);
#ifdef _WIN32
            strncpy(g_data_dir, ".", sizeof(g_data_dir));
#else
            strncpy(g_data_dir, ".", sizeof(g_data_dir));
#endif
            g_data_dir[sizeof(g_data_dir)-1] = '\0';
        }
    }
#ifdef _WIN32
    snprintf(g_arch_vehiculos, sizeof(g_arch_vehiculos), "%s\\vehiculos.dat", g_data_dir);
    snprintf(g_arch_precios,   sizeof(g_arch_precios),   "%s\\precios.dat",   g_data_dir);
#else
    snprintf(g_arch_vehiculos, sizeof(g_arch_vehiculos), "%s/vehiculos.dat", g_data_dir);
    snprintf(g_arch_precios,   sizeof(g_arch_precios),   "%s/precios.dat",   g_data_dir);
#endif
}

static int cargar_vehiculos(Vehiculo *arr, int maxn) {
    FILE *f = fopen(g_arch_vehiculos, "rb");
    if (!f) return 0;
    int n = (int)fread(arr, sizeof(Vehiculo), maxn, f);
    fclose(f);
    return n;
}

static void guardar_vehiculos(Vehiculo *arr, int n) {
    FILE *f = fopen(g_arch_vehiculos, "wb");
    if (!f) { perror("No se pudo guardar vehiculos"); return; }
    fwrite(arr, sizeof(Vehiculo), n, f);
    fclose(f);
}

static void guardar_precios(const PreciosCombustible *p) {
    FILE *f = fopen(g_arch_precios, "wb");
    if (!f) { perror("No se pudo guardar precios"); return; }
    fwrite(p, sizeof(PreciosCombustible), 1, f);
    fclose(f);
}
static int cargar_precios(PreciosCombustible *p) {
    FILE *f = fopen(g_arch_precios, "rb");
    if (!f) {
        p->precio_gasolina = 290.0f;
        p->precio_gasoil   = 224.0f;
        guardar_precios(p);
        return 0;
    }
    size_t ok = fread(p, sizeof(PreciosCombustible), 1, f);
    fclose(f);
    if (ok != 1) {
        p->precio_gasolina = 290.0f;
        p->precio_gasoil   = 224.0f;
        guardar_precios(p);
        return 0;
    }
    return 1;
}
static int siguiente_id(const Vehiculo *arr, int n) {
    int maxid = 0;
    for (int i = 0; i < n; ++i) if (arr[i].id > maxid) maxid = arr[i].id;
    return maxid + 1;
}

static int encontrar_por_id(const Vehiculo *arr, int n, int id) {
    for (int i = 0; i < n; ++i) if (arr[i].id == id) return i;
    return -1;
}

static int placa_duplicada(const Vehiculo *arr, int n, const char *placa, int ignorar_idx) {
#ifdef _WIN32
    for (int i = 0; i < n; ++i) {
        if (i == ignorar_idx) continue;
        if (!arr[i].activo) continue;
        const char *a = arr[i].placa;
        const char *b = placa;
        int diff = 0;
        while (*a && *b) {
            int ca = tolower((unsigned char)*a++);
            int cb = tolower((unsigned char)*b++);
            if (ca != cb) { diff = ca - cb; break; }
        }
        if (!diff && *a == '\0' && *b == '\0') return 1;
    }
    return 0;
#else
    for (int i = 0; i < n; ++i) {
        if (i == ignorar_idx) continue;
        if (arr[i].activo && strcasecmp(arr[i].placa, placa) == 0) return 1;
    }
    return 0;
#endif // _WIN32
}

static void listar_vehiculos(const Vehiculo *arr, int n) {
    int activos = 0;
    for (int i = 0; i < n; ++i) if (arr[i].activo) ++activos;
    printf("\n==== Vehiculos ====%s\n", activos==0?" (vacio)":"");
    for (int i = 0; i < n; ++i) {
        if (!arr[i].activo) continue;
        const Vehiculo *v = &arr[i];
        printf("ID:%d | %s %s | Placa:%s | Comb:%s | Carretera:%.2f km/gal | Ciudad:%.2f km/gal\n",
                v->id, v->marca, v->modelo, v->placa, v->tipo_combustible,
                v->km_por_galon_carretera, v->km_por_galon_ciudad);
    }
    printf("===================\n\n");
}

static void crear_vehiculo(Vehiculo *arr, int *n_ptr) {
    int n = *n_ptr;
    if (n >= MAX_VEHICULOS) { printf("\nCapacidad maxima alcanzada.\n\n"); return; }
    Vehiculo v; memset(&v, 0, sizeof(v));
    v.id = siguiente_id(arr, n);

    printf("\n--- Crear vehiculo ---\n");
    printf("Placa: "); leer_linea(v.placa, sizeof(v.placa));
    if (placa_duplicada(arr, n, v.placa, -1)) {
        printf("\nYa existe un vehiculo con esa placa. Operacion cancelada.\n\n");
        return;
    }
    printf("Marca: "); leer_linea(v.marca, sizeof(v.marca));
    printf("Modelo: "); leer_linea(v.modelo, sizeof(v.modelo));
    printf("Tipo Combustible (gasolina / gasoil|diesel): "); leer_linea(v.tipo_combustible, sizeof(v.tipo_combustible));
    v.km_por_galon_carretera = leer_float("Km x galon (Carretera): ", 0.01f, 10000.0f);
    v.km_por_galon_ciudad = leer_float("Km x galon (Ciudad): ", 0.01f, 10000.0f);
    v.costo_gomas = leer_float("Costo gomas (juego): ", 0.0f, 1e9f);
    v.km_gomas = leer_float("Km de gomas (vida util): ", 0.0f, 1e9f);
    v.costo_seguro_anual = leer_float("Costo seguro por 12 meses: ", 0.0f, 1e9f);
    v.costo_mantenimiento = leer_float("Costo mantenimiento: ", 0.0f, 1e9f);
    v.km_mantenimiento = leer_float("Cada cuantos km mantenimiento: ", 0.0f, 1e9f);
    v.costo_vehiculo = leer_float("Costo del vehiculo: ", 0.0f, 1e12f);
    v.vida_util_anios = leer_float("Vida util (anos): ", 0.0f, 100.0f);
    v.depreciacion_anual_pct = leer_float("Depreciacion anual (%) [0 para usar linea recta]: ", 0.0f, 100.0f);
    v.km_por_anio_promedio = leer_float("Km por ano promedio: ", 0.0f, 1e7f);
    v.activo = 1;

    int colocado = 0;
    for (int i=0; i<n; ++i) {
        if (!arr[i].activo) { arr[i] = v; colocado = 1; break; }
    }
    if (!colocado) arr[n++] = v;

    *n_ptr = n;
    guardar_vehiculos(arr, n);
    printf("\nVehiculo creado con ID %d.\n\n", v.id);
}
static void borrar_vehiculo(Vehiculo *arr, int n) {
    listar_vehiculos(arr, n);
    if (n == 0) return;
    int id = leer_int("Ingrese ID a borrar: ", 1, 1000000000);
    int idx = encontrar_por_id(arr, n, id);
    if (idx < 0 || !arr[idx].activo) { printf("\nNo encontrado.\n\n"); return; }
    arr[idx].activo = 0;
    guardar_vehiculos(arr, n);
    printf("\nVehiculo ID %d eliminado.\n\n", id);
}
static void modificar_vehiculo(Vehiculo *arr, int n) {
    listar_vehiculos(arr, n);
    if (n == 0) return;
    int id = leer_int("Ingrese ID a modificar: ", 1, 1000000000);
    int idx = encontrar_por_id(arr, n, id);
    if (idx < 0 || !arr[idx].activo) { printf("\nNo encontrado.\n\n"); return; }
    Vehiculo *v = &arr[idx];
    char linea[STRLEN];

    printf("\n--- Modificar vehiculo (enter para dejar igual) ---\n");
    printf("Placa actual [%s]: ", v->placa); leer_linea(linea, sizeof(linea)); if (strlen(linea)) {
        if (placa_duplicada(arr, n, linea, idx)) { printf("\nPlaca duplicada. Cancelado.\n\n"); return; }
        strncpy(v->placa, linea, sizeof(v->placa)); v->placa[sizeof(v->placa)-1]='\0';
    }
    printf("Marca actual [%s]: ", v->marca); leer_linea(linea, sizeof(linea)); if (strlen(linea)) { strncpy(v->marca, linea, sizeof(v->marca)); v->marca[sizeof(v->marca)-1]='\0'; }
    printf("Modelo actual [%s]: ", v->modelo); leer_linea(linea, sizeof(linea)); if (strlen(linea)) { strncpy(v->modelo, linea, sizeof(v->modelo)); v->modelo[sizeof(v->modelo)-1]='\0'; }
    printf("Tipo Combustible actual [%s]: ", v->tipo_combustible); leer_linea(linea, sizeof(linea)); if (strlen(linea)) { strncpy(v->tipo_combustible, linea, sizeof(v->tipo_combustible)); v->tipo_combustible[sizeof(v->tipo_combustible)-1]='\0'; }

    printf("Km x galon (Carretera) actual [%.2f]: ", v->km_por_galon_carretera); leer_linea(linea, sizeof(linea)); if (strlen(linea)) v->km_por_galon_carretera = strtof(linea, NULL);
    printf("Km x galon (Ciudad)    actual [%.2f]: ", v->km_por_galon_ciudad);    leer_linea(linea, sizeof(linea)); if (strlen(linea)) v->km_por_galon_ciudad    = strtof(linea, NULL);
    printf("Costo gomas (juego)     actual [%.2f]: ", v->costo_gomas);            leer_linea(linea, sizeof(linea)); if (strlen(linea)) v->costo_gomas            = strtof(linea, NULL);
    printf("Km de gomas (vida util) actual [%.2f]: ", v->km_gomas);               leer_linea(linea, sizeof(linea)); if (strlen(linea)) v->km_gomas               = strtof(linea, NULL);
    printf("Seguro anual            actual [%.2f]: ", v->costo_seguro_anual);     leer_linea(linea, sizeof(linea)); if (strlen(linea)) v->costo_seguro_anual     = strtof(linea, NULL);
    printf("Costo mantenimiento     actual [%.2f]: ", v->costo_mantenimiento);    leer_linea(linea, sizeof(linea)); if (strlen(linea)) v->costo_mantenimiento    = strtof(linea, NULL);
    printf("Km entre mantenimientos actual [%.2f]: ", v->km_mantenimiento);       leer_linea(linea, sizeof(linea)); if (strlen(linea)) v->km_mantenimiento       = strtof(linea, NULL);
    printf("Costo del vehiculo      actual [%.2f]: ", v->costo_vehiculo);         leer_linea(linea, sizeof(linea)); if (strlen(linea)) v->costo_vehiculo         = strtof(linea, NULL);
    printf("Vida util (anos)        actual [%.2f]: ", v->vida_util_anios);        leer_linea(linea, sizeof(linea)); if (strlen(linea)) v->vida_util_anios        = strtof(linea, NULL);
    printf("Deprec. anual (%%)      actual [%.2f]: ", v->depreciacion_anual_pct); leer_linea(linea, sizeof(linea)); if (strlen(linea)) v->depreciacion_anual_pct = strtof(linea, NULL);
    printf("Km por ano promedio     actual [%.2f]: ", v->km_por_anio_promedio);   leer_linea(linea, sizeof(linea)); if (strlen(linea)) v->km_por_anio_promedio   = strtof(linea, NULL);

    guardar_vehiculos(arr, n);
    printf("\nVehiculo ID %d modificado.\n\n", v->id);
}
static ResultadoViaje calcular_resultado(const Vehiculo *v, const PreciosCombustible *p, float km_viaje, float pct_ciudad) {
    ResultadoViaje r = {0};
    float frac_ciudad = pct_ciudad / 100.0f;
    if (frac_ciudad < 0) frac_ciudad = 0; if (frac_ciudad > 1) frac_ciudad = 1;
    float km_ciudad = km_viaje * frac_ciudad;
    float km_carretera = km_viaje - km_ciudad;

    float gal_ciudad = 0.0f, gal_carr = 0.0f;
    if (v->km_por_galon_ciudad > 0.0f)    gal_ciudad = km_ciudad / v->km_por_galon_ciudad;
    if (v->km_por_galon_carretera > 0.0f) gal_carr   = km_carretera / v->km_por_galon_carretera;
    float gal_total = gal_ciudad + gal_carr;
    float precio_choosen = es_gasoil(v->tipo_combustible) ? p->precio_gasoil : p->precio_gasolina;
    r.costo_combustible = gal_total * precio_choosen;

    if (v->km_gomas > 0.0f)
        r.costo_gomas = (v->costo_gomas / v->km_gomas) * km_viaje;

    if (v->km_por_anio_promedio > 0.0f)
        r.costo_seguro = (v->costo_seguro_anual / v->km_por_anio_promedio) * km_viaje;

    if (v->km_mantenimiento > 0.0f)
        r.costo_mantenimiento = (v->costo_mantenimiento / v->km_mantenimiento) * km_viaje;

    float deprec_anual_monetaria = 0.0f;
    if (v->depreciacion_anual_pct > 0.0f) {
        deprec_anual_monetaria = v->costo_vehiculo * (v->depreciacion_anual_pct / 100.0f);
    } else if (v->vida_util_anios > 0.0f) {
        deprec_anual_monetaria = v->costo_vehiculo / v->vida_util_anios;
    }
    if (v->km_por_anio_promedio > 0.0f && deprec_anual_monetaria > 0.0f) {
        float deprec_por_km = deprec_anual_monetaria / v->km_por_anio_promedio;
        r.costo_vehiculo = deprec_por_km * km_viaje;
    }
    r.total = r.costo_gomas + r.costo_seguro + r.costo_combustible + r.costo_mantenimiento + r.costo_vehiculo;
    if (km_viaje > 0.0f) r.costo_por_km = r.total / km_viaje;
    return r;
}
static void mostrar_resultado(const Vehiculo *v, const PreciosCombustible *p, float km, float pct_ciudad, const ResultadoViaje *r) {
    printf("\n========= Resultado desglosado =========\n");
    printf("Carpeta de datos: %s\n", g_data_dir);
    printf("Vehiculo: %s %s (Placa %s, Combustible: %s)\n", v->marca, v->modelo, v->placa, v->tipo_combustible);
    printf("Km del viaje: %.2f | %% en ciudad: %.2f%%\n", km, pct_ciudad);
    printf("----------------------------------------\n");
    printf("1) Costo de gomas:         %12.2f\n", r->costo_gomas);
    printf("2) Costo de seguro:        %12.2f\n", r->costo_seguro);
    printf("3) Costo de combustible:   %12.2f  (Gasolina: %.2f | Gasoil: %.2f)\n", r->costo_combustible, p->precio_gasolina, p->precio_gasoil);
    printf("4) Costo de mantenimiento: %12.2f\n", r->costo_mantenimiento);
    printf("5) Costo del vehiculo:     %12.2f\n", r->costo_vehiculo);
    printf("----------------------------------------\n");
    printf("Costo por km:              %12.4f\n", r->costo_por_km);
    printf("Costo total del viaje:     %12.2f\n", r->total);
    printf("========================================\n\n");
}
static void menu_gestion_vehiculos(Vehiculo *arr, int *n_ptr) {
    for (;;) {
        printf("\n- 1) Gestion de vehiculo -\n");
        printf("1 - Crear vehiculo\n");
        printf("2 - Borrar vehiculo\n");
        printf("3 - Modificar vehiculo\n");
        printf("4 - Listar vehiculos\n");
        printf("0 - Volver\n");
        int op = leer_int("Opcion: ", 0, 4);
        if (op == 0) return;
        switch (op) {
            case 1: crear_vehiculo(arr, n_ptr); break;
            case 2: borrar_vehiculo(arr, *n_ptr); break;
            case 3: modificar_vehiculo(arr, *n_ptr); break;
            case 4: listar_vehiculos(arr, *n_ptr); break;
        }
    }
}
static void menu_datos_generales(PreciosCombustible *precios) {
    for (;;) {
        printf("\n--- 2) Gestion de datos generales ---\n");
        printf("Carpeta de datos: %s\n", g_data_dir);
        printf("Precios actuales (por galon): Gasolina=%.2f | Gasoil=%.2f\n", precios->precio_gasolina, precios->precio_gasoil);
        printf("1 - Cambiar costo combustible Gasolina\n");
        printf("2 - Cambiar costo combustible Gasoil\n");
        printf("0 - Volver\n");
        int op = leer_int("Opcion: ", 0, 2);
        if (op == 0) return;
        if (op == 1) precios->precio_gasolina = leer_float("Nuevo precio Gasolina: ", 0.0f, 1e6f);
        if (op == 2) precios->precio_gasoil   = leer_float("Nuevo precio Gasoil: ",   0.0f, 1e6f);
        guardar_precios(precios);
        printf("\nGuardado.\n");
    }
}
static ResultadoViaje calcular_resultado(const Vehiculo *v, const PreciosCombustible *p, float km_viaje, float pct_ciudad);
static void mostrar_resultado(const Vehiculo *v, const PreciosCombustible *p, float km, float pct_ciudad, const ResultadoViaje *r);

static void menu_calculo_viaje(Vehiculo *arr, int n, const PreciosCombustible *precios) {
    if (n == 0) { printf("\nNo hay vehiculos cargados.\n\n"); return; }
    listar_vehiculos(arr, n);
    int id = leer_int("3.1) Seleccione ID de vehiculo: ", 1, 1000000000);
    int idx = encontrar_por_id(arr, n, id);
    if (idx < 0 || !arr[idx].activo) { printf("\nNo encontrado.\n\n"); return; }
    float km_viaje = leer_float("3.2) Kilometros del viaje: ", 0.01f, 1e8f);
    float pct_ciudad = leer_float("3.3) Porcentaje de km en ciudad (0-100): ", 0.0f, 100.0f);

    ResultadoViaje r = calcular_resultado(&arr[idx], precios, km_viaje, pct_ciudad);
    printf("\n3.4) Resultado desglosado:\n");
    mostrar_resultado(&arr[idx], precios, km_viaje, pct_ciudad, &r);
}
int main(void) {
    inicializar_directorio_datos();

    Vehiculo vehiculos[MAX_VEHICULOS];
    int nveh = cargar_vehiculos(vehiculos, MAX_VEHICULOS);

    PreciosCombustible precios;
    cargar_precios(&precios);

    for (;;) {
        printf("\n============= MENU PRINCIPAL =============\n");
        printf("Carpeta de datos: %s\n", g_data_dir);
        printf("1) Gestion de vehiculo\n");
        printf("2) Gestion de datos generales/(precio de combustible)\n");
        printf("3) Calculo costo viaje\n");
        printf("0) Salir\n");
        printf("==========================================\n");
        int op = leer_int("Elija una opcion: ", 0, 3);
        if (op == 0) { printf("\nHasta luego!\n"); break; }
        switch (op) {
            case 1: menu_gestion_vehiculos(vehiculos, &nveh); break;
            case 2: menu_datos_generales(&precios); break;
            case 3: menu_calculo_viaje(vehiculos, nveh, &precios); break;
            default: break;
        }
    }
    guardar_vehiculos(vehiculos, nveh);
    guardar_precios(&precios);
    return 0;
}
