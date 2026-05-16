# BitLab-MANUS  
**Motion-capture ARduino Neural Unified System**

Projeto de mão robótica controlada por gestos, combinando visão computacional com Arduino. Desenvolvido no BitLab/UFOP para fins didáticos e de extensão.

---

## 🎯 Objetivo

Capturar, interpretar e replicar em tempo real os movimentos da mão humana em uma mão robótica, utilizando:

- Visão computacional com detecção de pontos-chave da mão
- Comunicação serial com Arduino
- Servomotores para replicação física

---

## ⚙️ Tecnologias Utilizadas

- **Python (PC/Host)**: OpenCV, NumPy, rede neural (Caffe)
- **Arduino**: controle dos servos
- **Modelo de detecção de mão**:  
  - `pose_iter_102000.caffemodel`  
  - `pose_deploy.prototxt`

---

## ⚠️ Sobre os Arquivos de Modelo

Por limitação do GitHub (100 MB), os arquivos `.caffemodel` e `.prototxt` **não estão inclusos no repositório**.

Você pode baixá-los aqui:  
🔗 https://drive.google.com/drive/folders/1FFQdfEqxwK0icTlshhNu0BgmZC7mejmO?usp=sharing

**Após o download**, coloque os arquivos na pasta:

Alteração - Thainan


```bash
src/host/hand/

