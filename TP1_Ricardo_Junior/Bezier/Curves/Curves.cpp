/**********************************************************************************
// Curves (Código Fonte)
//
// Descrição:  TRABALHO PRIMEIRA UNIDADE -> COMPUTAÇÃO GRÁFICA
// Aluno:      Ricardo Cezar Fernandes de Melo Junior
//
**********************************************************************************/

#include "Bezier.h"

// ------------------------------------------------------------------------------
//                                  WinMain                                      
// ------------------------------------------------------------------------------

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
    try
    {
        // cria motor e configura a janela
        Engine* engine = new Engine();
        engine->window->Mode(WINDOWED);
        engine->window->Size(1024, 600);
        engine->window->ResizeMode(ASPECTRATIO);
        engine->window->Color(0, 0, 0);
        engine->window->Title("Trabalho 01 -> Curvas de Bézier");
        engine->window->Icon(IDI_ICON);
        engine->window->LostFocus(Engine::Pause);
        engine->window->InFocus(Engine::Resume);

        // cria e executa a aplicação
        engine->Start(new Bezier());

        // finaliza execução
        delete engine;
    }
    catch (Error& e)
    {
        // exibe mensagem em caso de erro
        MessageBox(nullptr, e.ToString().data(), "Curves", MB_OK);
    }

    return 0;
}

// ----------------------------------------------------------------------------
