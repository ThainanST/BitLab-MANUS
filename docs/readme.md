# Projeto Original – Controle de Dedo Mecânico com Visão Computacional

Este projeto foi a base conceitual para o BitLab-MANUS. Ele demonstra como a **visão computacional baseada em aprendizado profundo** pode ser usada para **controlar, em tempo real, um atuador físico (servo motor)** a partir do movimento do dedo humano captado por uma webcam comum.

## 🎯 Objetivo

Controlar um **servo motor conectado a um Arduino** usando os dados da **posição do dedo indicador** detectado em tempo real pela câmera, com base em um modelo de pose de mão treinado via Deep Learning.

---

## 📚 Base Técnica

O projeto utiliza uma rede neural treinada para detectar **22 pontos chave (keypoints)** da mão em imagens RGB, conforme a arquitetura descrita no artigo:

> Simon, T. et al. (2017). *Hand Keypoint Detection in Single Images using Multiview Bootstrapping*. Carnegie Mellon University.

A rede é baseada em um modelo do tipo **Convolutional Pose Machines (CPM)**, treinado com o processo de *bootstrapping multiview*, que melhora iterativamente a detecção de pontos ao combinar vistas múltiplas e reprojeções trianguladas em 3D para gerar novos dados anotados.

---

## 🛠️ Implementação

### 📷 Captura e Detecção (PC - Python + OpenCV)

- A webcam captura o vídeo em tempo real.
- O modelo Caffe (`pose_iter_102000.caffemodel`) detecta os 22 pontos da mão.
- O sistema rastreia dois pontos:
  - **Ponto 5:** base do dedo indicador
  - **Ponto 8:** ponta do dedo indicador
- Calcula-se a distância euclidiana entre esses pontos como medida da "flexão" do dedo.

### 🔁 Mapeamento

- A distância medida é convertida em um valor de 0 a 180 graus, proporcional à flexão.
- O valor resultante é transmitido via **porta serial (USB)** ao Arduino.

### ⚙️ Atuação (Arduino)

- O Arduino recebe os dados via serial e comanda um **servo motor SG90**, posicionando o dedo mecânico de acordo com a flexão detectada.

---

## 🔬 Precisão e Suavização

- O código aplica **filtros de suavização exponencial** para evitar jitter nas coordenadas dos pontos detectados.
- Os dados são salvos em tempo real para `output.txt` e um vídeo é gerado com as detecções sobrepostas.

---

## 📎 Arquivos Relacionados

- `handPoseVideoPJ.py` – Script completo de captura, detecção e envio serial
- `pose_deploy.prototxt` – Arquitetura da rede
- `pose_iter_102000.caffemodel` – Pesos treinados
- `output.avi` – Vídeo gravado com esqueleto detectado
- [`docs/(2017)[Simon][Hand Keypoint Detection...].pdf`](./(2017)[Simon][Hand%20Keypoint%20Detection%20in%20Single%20Images%20using%20Multiview%20Bootstrapping].pdf) – Artigo completo com os detalhes da abordagem original

---

## ⚠️ Limitações

- Funciona apenas para um dedo (indicador)
- Requer iluminação razoável e ângulo adequado
- Não há retorno ou verificação da resposta física (atuador)

---

## 🔗 Link do Vídeo Demonstrativo

▶️ [YouTube – Visão de Máquina Controla Dedo Mecânico com Arduino](https://www.youtube.com/watch?v=_PCG8o3Px_A)
