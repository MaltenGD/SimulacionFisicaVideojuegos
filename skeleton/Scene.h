#pragma once

#include <string>
#include "PxPhysicsAPI.h"

//Para evitar ambiguedad, ya que PxPhysicsAPI implementa una clase tambi�n llamada Camera.
namespace Snippets {
    class Camera;
}
using Snippets::Camera;

// Clase base para las distintas escenas de la aplicaci�n.
// Provee la interfaz m�nima que debe implementar cualquier escena:
// inicializaci�n, limpieza, actualizaci�n por frame y manejo de teclado.
class Scene {
public:
    // Construye la escena con un nombre identificador.
    explicit Scene(std::string name) : m_name(std::move(name)) {}
    virtual ~Scene() = default;

    // Inicializa recursos de la escena (f�sicos, gr�ficos, datos, ...).
    virtual void init() = 0;

    // Libera los recursos asignados en init().
    virtual void cleanup() = 0;

    // Actualiza la l�gica de la escena.
    // dt: tiempo en segundos transcurrido desde la �ltima actualizaci�n.
    virtual void update(double dt) = 0;

    // Manejo de pulsaci�n de tecla.
    // Se recibe la tecla pulsada y la transformada de la c�mara para
    // permitir respuestas dependientes de la orientaci�n/posici�n de la c�mara.
    // M�todo opcional que puede ser sobrescrito por escenas que lo necesiten.
    virtual void keyPress(unsigned char key, Camera* camera) {}


    //Dibuja la UI de Dear ImGUI
    //Las escenas que no son de tipo Sandbox dejan esto vacio.
    virtual void drawIMGUI() {}

    // Devuelve el nombre identificador de la escena.
    [[nodiscard]] const std::string& getName() const { return m_name; }

protected:
    // Nombre de la escena (�til para identificarla en men�s o logs).
    std::string m_name;
};