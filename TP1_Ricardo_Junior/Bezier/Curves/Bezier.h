#pragma once
/**********************************************************************************
// Bezier (Código Fonte)
//
// Descrição:  TRABALHO PRIMEIRA UNIDADE -> COMPUTAÇÃO GRÁFICA
// Aluno:      Ricardo Cezar Fernandes de Melo Junior
//
**********************************************************************************/

#ifndef DXUT_BEZIER_H
#define DXUT_BEZIER_H

// ---------------------------------------------------------------------------------

#include "Graphics.h"
#include "Window.h"
#include "Input.h"
#include "DXUT.h"
#include <fstream>   // Para manipulação de arquivos (salvar e carregar)
#include <string>    // Para trabalhar com strings
#include <sstream>   // Para manipulação de strings (separação de valores)
#include <iostream>  
// ---------------------------------------------------------------------------------

struct Vertex
{
    XMFLOAT3 Pos;
    XMFLOAT4 Color;
};

// ---------------------------------------------------------------------------------

// ESTRUTURA PARA ARMAZENAR ESTADO DAS CURVAS
struct BezierState {
    uint contadorCurvas;                                                // QUANTIDADE DE CURVAS
    float posX[4];                                                      // POSIÇÕES DE X
    float posY[4];                                                      // POSIÇÕES DE Y
};

// ---------------------------------------------------------------------------------

class Bezier : public App
{
private:
    static const uint MaxVertex = 2048;
    static const uint estadoMaxSize = 20;                               // TAMANHO DO VETOR DE ESTADO
    const float offset = 4.0f;                                          // CONSTANTE DO ESPAÇAMENTO DOS QUADRADOS

    ID3D12RootSignature* rootSignature;
    ID3D12PipelineState* pipelineState;
    Mesh* geometry;

    Mesh* quadrados[4];                                                 // MALHA DOS QUADRADOS DE CONTROLE
    Mesh* linhas[2];                                                    // MALHA DAS LINHAS DE CONTROLE

    Vertex pontosQuadrados[4][6];                                       // PONTOS PARA DESENHAR O OBJETO QUADRADO DE CONTROLE
    Vertex pontosLinhas[2][4];                                          // PONTOS PARA DESENHAR O OBJETO LINHA DE CONTROLE
    Vertex vertices[MaxVertex];
    
    float pontosGuiaX[4];                                               // COORDENADAS DAS POSIÇÕES DOS OBJETOS DE GUIA NO EIXO X
    float pontosGuiaY[4];                                               // COORDENADAS DAS POSIÇÕES DOS OBJETOS DE GUIA NO EIXO Y

    uint click;                                                         // QUANTIDADE DE CLICKS, OU SEJA, QUANTIDADE DE PONTOS
    uint linhasDesenhadas;                                              // NÚMERO DE LINHAS DESENHADAS
    int indexEstado;                                                    // MARCADOR DO VETOR DE ESTADO
    uint contador;
    uint index;
    
    BezierState estado[estadoMaxSize];                                  // VETOR ESTADO, NA QUAL ARMAZENA AS POSIÇÕES DAS CURVAS
    BezierState estadoSalvo;                                            // ESTADO QUE PEGA A ULTIMA CURVA SALVA

public:
    void Init();
    void Update();
    void Display();
    void Finalize();

    void Limpar();                                                      // METODO DE LIMPAR A TELA
    void QuadradoGuia(uint index, float cx, float cy);                  // INSERE OS VERTICES DOS QUADRADOS DE CONTROLE GUIA
    void LinhaGuia(uint index, bool flag);                              // INSERE OS VERTICES DAS LINHAS DE CONTROLE GUIA
    void CriarCurva();                                                  // METODO QUE CRIA A CURVA
    void Salvar();                                                      // METODO QUE SALVA OS ESTADOS DA CURVA
    void Restaurar();                                                   // METODO QUE RESTAURA OS ESTADOS DA CURVA

    void BuildRootSignature();
    void BuildPipelineState();
};

// ---------------------------------------------------------------------------------

#endif