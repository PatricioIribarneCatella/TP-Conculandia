// Unico por persona
enum colores_ojos {
    OJOS_VERDES,
    OJOS_AZULES,
    OJOS_MARRONES_OSCUROS,
    OJOS_MARRONES_CLAROS,
    OTRO_OJOS
};
#define MAX_COLOR_OJO OTRO_OJOS


// Unico por persona
enum tipos_pelos {
    PELO_RUBIO,
    PELO_MOROCHO,
    PELO_CASTANIO, 
    PELO_COLORADO,
    PELO_CANOSO,
    PELADO,
    OTRO_PELO
};
#define MAX_TIPO_PELO OTRO_PELO

// Varios por persona, indica N°bit donde esta marcado
enum caracteristicas_especiales {
    GORRA,
    TATUAJE_EN_PIERNAS,
    TATUAJE_EN_ROSTRO,
    AROS,
    PIERCING_EN_NARIZ,
    PIERCING_EN_BOCA,
    PIERCING_EN_OTRO_LADO
};
#define MAX_CARACTERISTICA_ESPECIAL PIERCING_EN_OTRO_LADO

// Enum 
enum sexos {
    MUJER,
    HOMBRE,
    OTRO_SEXO
};
#define MAX_SEXO OTRO_SEXO



#define CAMBIO_OK 0
#define ARGUMENTO_INVALIDO -1

typedef struct {
    enum colores_ojos ojos;
    enum tipos_pelos pelo;
    enum caracteristicas_especiales caracteristicasEspeciales;
    enum sexos sexo;
} CaracteristicasPersona;


int Cambiar_sexo(CaracteristicasPersona *car, enum sexos nuevo_sexo);
int Cambiar_pelo(CaracteristicasPersona *car, enum tipos_pelos nuevo_pelo);
int Aniadir_caracteristica_especial(CaracteristicasPersona *car, enum caracteristicas_especiales nueva_caracteristica);
int Quitar_caracteristica_especial(CaracteristicasPersona *car, enum caracteristicas_especiales caracteristica);
int Vaciar_caracteristicas_especiales(CaracteristicasPersona *car);
