# Manual de Instalação e Execução - TP2

* **Disciplina:** BCC423 Criptografia e Segurança de Redes  
* **Professor:** Dr. Carlos Frederico Marcelo da Cunha Cavalacanti  
* **Sistema Operacional:** Windows 10/11  
* **Navegador:** Google Chrome  

Este documento contém os passos necessários para configurar o host nativo, carregar a extensão no navegador e utilizar a interface de seleção de certificados.

---

## Instalação do Host Nativo e Extensão

Como o Chrome gera um ID único para a extensão ao ser carregada em uma nova máquina, a instalação deve seguir esta ordem específica para que a comunicação Native Messaging funcione corretamente.

### Passo 1: Carregar a extensão no Chrome (Modo Desenvolvedor)
1. Abra o Google Chrome e acesse `chrome://extensions/`.
2. No canto superior direito, ative o **Modo do desenvolvedor**
3. Clique no botão **Carregar sem compactação**
4. Selecione a pasta que contém os arquivos da extensão (`manifest.json`, `popup.html`, `popup.js`, etc.).
5. Após o carregamento, a extensão aparecerá na lista. **Copie o ID gerado** (uma sequência longa de letras).

### Passo 2: Configurar o arquivo de manifesto do Native Messaging
1. Abra o arquivo `nm_manifest.json` em um editor de texto.
2. Na propriedade `"path"`, insira o **caminho absoluto** atualizado para o arquivo `host_nativo.exe` na sua máquina.
3. Na propriedade `"allowed_origins"`, substitua o ID existente pelo ID que você acabou de copiar no Passo 1. O formato deve ficar assim: `"chrome-extension://SEU_ID/"`.
4. Salve o arquivo.

### Passo 3: Registrar o Host Nativo no Windows 
1. Navegue até a pasta onde o arquivo `install.bat` está localizado.
2. Clique com o botão direito sobre o arquivo `install.bat` e selecione **Executar como Administrador**.
3. Uma janela do prompt de comando será aberta confirmando a criação da chave no registro do Windows. Isso informará ao Chrome o local exato do arquivo `nm_manifest.json`.

---

