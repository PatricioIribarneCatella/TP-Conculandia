
#include "CaracteristicasPersona.h"

int Cambiar_sexo(CaracteristicasPersona *car, enum sexos nuevo_sexo) {
    if (nuevo_sexo > MAX_SEXO)
        return ARGUMENTO_INVALIDO;
    car->sexo = nuevo_sexo;
    return CAMBIO_OK;

}

int Cambiar_pelo(CaracteristicasPersona *car, enum tipos_pelos nuevo_pelo) {
    if (nuevo_pelo > MAX_TIPO_PELO)
        return ARGUMENTO_INVALIDO;
    car->pelo = nuevo_pelo;
    return CAMBIO_OK;

}

int Cambiar_ojos(CaracteristicasPersona *car, enum colores_ojos nuevos_ojos) {
    if (nuevos_ojos > MAX_COLOR_OJO)
        return ARGUMENTO_INVALIDO;
    car->ojos = nuevos_ojos;
    return CAMBIO_OK;

}

int Aniadir_caracteristica_especial(CaracteristicasPersona *car, enum caracteristicas_especiales nueva_caracteristica) {
    if (nueva_caracteristica > MAX_CARACTERISTICA_ESPECIAL)
        return ARGUMENTO_INVALIDO;
    car->caracteristicasEspeciales = car->caracteristicasEspeciales | (1<<nueva_caracteristica);
    return CAMBIO_OK;
}

int Quitar_caracteristica_especial(CaracteristicasPersona *car, enum caracteristicas_especiales caracteristica) {
    if (caracteristica > MAX_CARACTERISTICA_ESPECIAL)
        return ARGUMENTO_INVALIDO;
    car->caracteristicasEspeciales = car->caracteristicasEspeciales & ~(1<<caracteristica);
    return CAMBIO_OK;

}

int Vaciar_caracteristicas_especiales(CaracteristicasPersona *car) {
    car->caracteristicasEspeciales = 0;
    return CAMBIO_OK;
}