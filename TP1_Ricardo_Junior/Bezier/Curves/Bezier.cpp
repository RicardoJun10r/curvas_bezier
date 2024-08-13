/**********************************************************************************
// Bezier (Código Fonte)
//
// Descrição:  TRABALHO PRIMEIRA UNIDADE -> COMPUTAÇÃO GRÁFICA
// Aluno:      Ricardo Cezar Fernandes de Melo Junior
//
**********************************************************************************/
#include "Bezier.h"

// ------------------------------------------------------------------------------

void Bezier::Init()
{
    graphics->ResetCommands();

    // ---------[ Build Geometry ]------------

    // tamanho do buffer de vértices em bytes
    const uint vbSize = MaxVertex * sizeof(Vertex);

    // cria malha 3D
    geometry = new Mesh(vbSize, sizeof(Vertex));

    // ---------------------------------------

    // INICIANDO AS MALHAS DOS OBJETOS QUADRADO E LINHA
    for (int i = 0; i < 4; ++i){ quadrados[i] = new Mesh((6 * sizeof(Vertex)), sizeof(Vertex)); }

    for (int i = 0; i < 2; ++i){ linhas[i] = new Mesh((4 * sizeof(Vertex)), sizeof(Vertex)); }

    click = 1;
    indexEstado = -1;
    linhasDesenhadas = 0;
    contador = 0;
    index = 0;

    // ---------------------------------------

    BuildRootSignature();
    BuildPipelineState();

    // ---------------------------------------

    graphics->SubmitCommands();
}

// ------------------------------------------------------------------------------

void Bezier::Limpar()
{
    geometry = new Mesh((MaxVertex * sizeof(Vertex)), sizeof(Vertex));

    // QUADRADO
    for (int i = 0; i < 4; i++) { quadrados[i] = new Mesh((6 * sizeof(Vertex)), sizeof(Vertex)); }
    
    // LINHA
    for (int i = 0; i < 2; i++) { linhas[i] = new Mesh((4 * sizeof(Vertex)), sizeof(Vertex)); }

    click = 1;
    index = 0;
    contador = 0;
    linhasDesenhadas = 0;

    graphics->ResetCommands();
    graphics->Copy(vertices, geometry->vertexBufferSize, geometry->vertexBufferUpload, geometry->vertexBufferGPU);
    graphics->SubmitCommands();
    Display();
}

// ------------------------------------------------------------------------------

void Bezier::Update()
{
    // COORDENADAS DE TELA E MOUSE
    float cx = float(window->CenterX());
    float cy = float(window->CenterY());
    float mx = float(input->MouseX());
    float my = float(input->MouseY());

    // NORMALIZANDO AS COORDENADAS
    float normalizadoX = (mx - cx) / cx;
    float normalizadoY = (cy - my) / cy;

    // sai com o pressionamento da tecla ESC
    if (input->KeyPress(VK_ESCAPE))
        window->Close();

    // LIMPAR TELA
    if (input->KeyPress(VK_DELETE))
        Limpar();

    // SALVAR ESTADO
    if (input->KeyPress('S'))
        Salvar();

    // RESTAURAR ESTADO
    if (input->KeyPress('L'))
        Restaurar();

    // SE ESTIVER NO QUARTO PONTO, DESENHAR A CURVA
    if (click == 4) {

        pontosGuiaX[3] = pontosGuiaX[2] - (pontosGuiaX[2] - normalizadoX);
        pontosGuiaY[3] = pontosGuiaY[2] - (pontosGuiaY[2] - normalizadoY);

        // DESENHAR QUADRADO DE CONTROLE
        QuadradoGuia(3, cx, cy);
        
        // DESENHA LINHA DE CONTROLE
        LinhaGuia(1, false);

        index = linhasDesenhadas * 50;
        contador = index;

        // DESENHA A CURVA DE BEZIER
        CriarCurva();

        graphics->ResetCommands();
        graphics->Copy(pontosQuadrados[3], quadrados[3]->vertexBufferSize, quadrados[3]->vertexBufferUpload, quadrados[3]->vertexBufferGPU);
        graphics->Copy(pontosLinhas[1], linhas[1]->vertexBufferSize, linhas[1]->vertexBufferUpload, linhas[1]->vertexBufferGPU);
        graphics->Copy(vertices, geometry->vertexBufferSize, geometry->vertexBufferUpload, geometry->vertexBufferGPU);
        graphics->SubmitCommands();
        Display();
    }

    // cria vértices com o botão do mouse
    if (input->KeyPress(VK_LBUTTON))
    {
        switch (click)
        {
        case 1:
        {
            pontosGuiaX[0] = normalizadoX;
            pontosGuiaY[0] = normalizadoY;

            QuadradoGuia(0, cx, cy);

            graphics->ResetCommands();
            graphics->Copy(pontosQuadrados[0], quadrados[0]->vertexBufferSize, quadrados[0]->vertexBufferUpload, quadrados[0]->vertexBufferGPU);
            graphics->SubmitCommands();

            click++;
            Display();
            break;
        }
        case 2:
        {
            pontosGuiaX[1] = pontosGuiaX[0] - (pontosGuiaX[0] - normalizadoX);
            pontosGuiaY[1] = pontosGuiaY[0] - (pontosGuiaY[0] - normalizadoY);

            // DESENHAR QUADRADO DE CONTROLE
            QuadradoGuia(1, cx, cy);

            // DESENHA LINHA DE CONTROLE
            LinhaGuia(0, true);

            graphics->ResetCommands();
            graphics->Copy(pontosQuadrados[1], quadrados[1]->vertexBufferSize, quadrados[1]->vertexBufferUpload, quadrados[1]->vertexBufferGPU);
            graphics->Copy(pontosLinhas[0], linhas[0]->vertexBufferSize, linhas[0]->vertexBufferUpload, linhas[0]->vertexBufferGPU);
            graphics->SubmitCommands();

            click++;
            Display();
            break;
        }
        case 3:
        {
            pontosGuiaX[2] = normalizadoX;
            pontosGuiaY[2] = normalizadoY;

            // DESENHAR QUADRADO DE CONTROLE
            QuadradoGuia(2, cx, cy);

            graphics->ResetCommands();
            graphics->Copy(pontosQuadrados[2], quadrados[2]->vertexBufferSize, quadrados[2]->vertexBufferUpload, quadrados[2]->vertexBufferGPU);
            graphics->SubmitCommands();

            click++;
            Display();
            break;
        }
        case 4:
        {
            linhasDesenhadas++;

            pontosGuiaX[0] = pontosGuiaX[3];
            pontosGuiaY[0] = pontosGuiaY[3];

            pontosGuiaX[1] = pontosGuiaX[3] - (pontosGuiaX[2] - pontosGuiaX[3]);
            pontosGuiaY[1] = pontosGuiaY[3] - (pontosGuiaY[2] - pontosGuiaY[3]);

            // DESENHAR QUADRADO DE CONTROLE
            QuadradoGuia(0, cx, cy);

            graphics->ResetCommands();
            graphics->Copy(pontosQuadrados[0], quadrados[0]->vertexBufferSize, quadrados[0]->vertexBufferUpload, quadrados[0]->vertexBufferGPU);
            graphics->Copy(pontosQuadrados[1], quadrados[1]->vertexBufferSize, quadrados[1]->vertexBufferUpload, quadrados[1]->vertexBufferGPU);
            graphics->Copy(pontosLinhas[0], linhas[0]->vertexBufferSize, linhas[0]->vertexBufferUpload, linhas[0]->vertexBufferGPU);
            graphics->SubmitCommands();

            click--;
            Display();
            break;
        }
        default:
            break;
        }
    }
}

// ------------------------------------------------------------------------------

void Bezier::CriarCurva()
{
    for (float t = 0; t < 1; t += 0.02) {

        float pow0 = pow((1 - t), 3.0);
        float pow1 = pow((1 - t), 2.0);

        float pow2 = pow(t, 3.0);
        float pow3 = pow(t, 2.0);

        float x = ((pow0 * pontosGuiaX[0]) + (3 * t * pow1 * pontosGuiaX[1]) + (3 * pow3 * (1 - t) * pontosGuiaX[2]) + (pow2 * pontosGuiaX[3]));
        float y = ((pow0 * pontosGuiaY[0]) + (3 * t * pow1 * pontosGuiaY[1]) + (3 * pow3 * (1 - t) * pontosGuiaY[2]) + (pow2 * pontosGuiaY[3]));

        vertices[index] = { XMFLOAT3(x, y, 0.0f), XMFLOAT4(Colors::Yellow) };
        index = (index + 1) % MaxVertex;
        contador++;
    }
}

// ------------------------------------------------------------------------------

void Bezier::QuadradoGuia(uint index, float cx, float cy)
{
    for (int i = 0; i < 6; ++i) {
        float paddingX = (i >= 1 && i <= 3) ? offset / cx : -offset / cx;
        float paddingY = (i >= 2 && i <= 4) ? offset / cy : -offset / cy;

        pontosQuadrados[index][i] = { XMFLOAT3(pontosGuiaX[index] + paddingX, pontosGuiaY[index] + paddingY, 0.0f), XMFLOAT4(Colors::Red) };
    }
}

// ------------------------------------------------------------------------------

void Bezier::LinhaGuia(uint index, bool flag)
{
    if (flag)
    {
        for (int i = 0; i < 4; ++i) {
            pontosLinhas[index][i] = { XMFLOAT3(pontosGuiaX[index], pontosGuiaY[index], 0.0f), XMFLOAT4(Colors::Red) };
        }
    }
    else
    {
        for (int i = 0; i < 4; ++i) {
            pontosLinhas[index][i] = { XMFLOAT3(pontosGuiaX[i], pontosGuiaY[i], 0.0f), XMFLOAT4(Colors::Red) };
        }
    }
}

// ------------------------------------------------------------------------------

void Bezier::Salvar()
{
    // VERIFICA SE AINDA TEM ESPAÇO PARA SALVAR NO VETOR
    if (indexEstado < estadoMaxSize - 1) indexEstado++;
    else indexEstado = estadoMaxSize - 1;

    // SALVANDO AS COORDENADAS DAS CURVAS NO VETOR ESTADO
    for (int i = 0; i < 4; i++) {
       estado[indexEstado].posX[i] = pontosGuiaX[i];
       estado[indexEstado].posY[i] = pontosGuiaY[i];
    }
    estado[indexEstado].contadorCurvas = linhasDesenhadas;

    estadoSalvo = estado[indexEstado];
}

// ------------------------------------------------------------------------------

void Bezier::Restaurar()
{
    if (indexEstado >= 0)
    {
        indexEstado--;

        // PEGANDO AS COORDENADAS SALVAS
        for (int i = 0; i < 4; i++) {
            pontosGuiaX[i] = estadoSalvo.posX[i];

            pontosGuiaY[i] = estadoSalvo.posY[i];
        }

        linhasDesenhadas = estadoSalvo.contadorCurvas;

        // LIMPANDO AS CURVAS ATUAIS
        graphics->ResetCommands();
        graphics->SubmitCommands();

        Display();
    }
}

// ------------------------------------------------------------------------------

void Bezier::Display()
{
    // limpa backbuffer
    graphics->Clear(pipelineState);

    // submete comandos de configuração do pipeline
    graphics->CommandList()->SetGraphicsRootSignature(rootSignature);
    graphics->CommandList()->IASetVertexBuffers(0, 1, geometry->VertexBufferView());
    graphics->CommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP);

    // submete comandos de desenho
    graphics->CommandList()->DrawInstanced(contador, 1, 0, 0);


    // QUADRADOS
    for (int i = 0; i < 4; i++)
    {
        graphics->CommandList()->IASetVertexBuffers(0, 1, quadrados[i]->VertexBufferView());
        graphics->CommandList()->DrawInstanced(6, 1, 0, 0);
    }

    // LINHAS
    for (int i = 0; i < 2; i++)
    {
        graphics->CommandList()->IASetVertexBuffers(0, 1, linhas[i]->VertexBufferView());
        graphics->CommandList()->DrawInstanced(4, 1, 0, 0);
    }

    // apresenta backbuffer
    graphics->Present();
}

// ------------------------------------------------------------------------------

void Bezier::Finalize()
{
    rootSignature->Release();
    pipelineState->Release();
    delete geometry;
}

// ------------------------------------------------------------------------------
//                                     D3D                                      
// ------------------------------------------------------------------------------

void Bezier::BuildRootSignature()
{
    // descrição para uma assinatura vazia
    D3D12_ROOT_SIGNATURE_DESC rootSigDesc = {};
    rootSigDesc.NumParameters = 0;
    rootSigDesc.pParameters = nullptr;
    rootSigDesc.NumStaticSamplers = 0;
    rootSigDesc.pStaticSamplers = nullptr;
    rootSigDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

    // serializa assinatura raiz
    ID3DBlob* serializedRootSig = nullptr;
    ID3DBlob* error = nullptr;

    ThrowIfFailed(D3D12SerializeRootSignature(
        &rootSigDesc,
        D3D_ROOT_SIGNATURE_VERSION_1,
        &serializedRootSig,
        &error));

    // cria uma assinatura raiz vazia
    ThrowIfFailed(graphics->Device()->CreateRootSignature(
        0,
        serializedRootSig->GetBufferPointer(),
        serializedRootSig->GetBufferSize(),
        IID_PPV_ARGS(&rootSignature)));
}

// ------------------------------------------------------------------------------

void Bezier::BuildPipelineState()
{
    // --------------------
    // --- Input Layout ---
    // --------------------

    D3D12_INPUT_ELEMENT_DESC inputLayout[2] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
    };

    // --------------------
    // ----- Shaders ------
    // --------------------

    ID3DBlob* vertexShader;
    ID3DBlob* pixelShader;

    D3DReadFileToBlob(L"Shaders/Vertex.cso", &vertexShader);
    D3DReadFileToBlob(L"Shaders/Pixel.cso", &pixelShader);

    // --------------------
    // ---- Rasterizer ----
    // --------------------

    D3D12_RASTERIZER_DESC rasterizer = {};
    rasterizer.FillMode = D3D12_FILL_MODE_WIREFRAME;
    rasterizer.CullMode = D3D12_CULL_MODE_NONE;
    rasterizer.FrontCounterClockwise = FALSE;
    rasterizer.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
    rasterizer.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
    rasterizer.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
    rasterizer.DepthClipEnable = TRUE;
    rasterizer.MultisampleEnable = FALSE;
    rasterizer.AntialiasedLineEnable = FALSE;
    rasterizer.ForcedSampleCount = 0;
    rasterizer.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

    // ---------------------
    // --- Color Blender ---
    // ---------------------

    D3D12_BLEND_DESC blender = {};
    blender.AlphaToCoverageEnable = FALSE;
    blender.IndependentBlendEnable = FALSE;
    const D3D12_RENDER_TARGET_BLEND_DESC defaultRenderTargetBlendDesc =
    {
        FALSE,FALSE,
        D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
        D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
        D3D12_LOGIC_OP_NOOP,
        D3D12_COLOR_WRITE_ENABLE_ALL,
    };
    for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
        blender.RenderTarget[i] = defaultRenderTargetBlendDesc;

    // ---------------------
    // --- Depth Stencil ---
    // ---------------------

    D3D12_DEPTH_STENCIL_DESC depthStencil = {};
    depthStencil.DepthEnable = TRUE;
    depthStencil.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
    depthStencil.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
    depthStencil.StencilEnable = FALSE;
    depthStencil.StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK;
    depthStencil.StencilWriteMask = D3D12_DEFAULT_STENCIL_WRITE_MASK;
    const D3D12_DEPTH_STENCILOP_DESC defaultStencilOp =
    { D3D12_STENCIL_OP_KEEP, D3D12_STENCIL_OP_KEEP, D3D12_STENCIL_OP_KEEP, D3D12_COMPARISON_FUNC_ALWAYS };
    depthStencil.FrontFace = defaultStencilOp;
    depthStencil.BackFace = defaultStencilOp;

    // -----------------------------------
    // --- Pipeline State Object (PSO) ---
    // -----------------------------------

    D3D12_GRAPHICS_PIPELINE_STATE_DESC pso = {};
    pso.pRootSignature = rootSignature;
    pso.VS = { reinterpret_cast<BYTE*>(vertexShader->GetBufferPointer()), vertexShader->GetBufferSize() };
    pso.PS = { reinterpret_cast<BYTE*>(pixelShader->GetBufferPointer()), pixelShader->GetBufferSize() };
    pso.BlendState = blender;
    pso.SampleMask = UINT_MAX;
    pso.RasterizerState = rasterizer;
    pso.DepthStencilState = depthStencil;
    pso.InputLayout = { inputLayout, 2 };
    pso.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
    pso.NumRenderTargets = 1;
    pso.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
    pso.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
    pso.SampleDesc.Count = graphics->Antialiasing();
    pso.SampleDesc.Quality = graphics->Quality();
    graphics->Device()->CreateGraphicsPipelineState(&pso, IID_PPV_ARGS(&pipelineState));

    vertexShader->Release();
    pixelShader->Release();
}
