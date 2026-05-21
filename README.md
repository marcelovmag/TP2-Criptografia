# Manual de Instalação e Execução - TP2

**Disciplina:** BCC423 Criptografia e Segurança de Redes 
**Professor:** Dr. Carlos Frederico Marcelo da Cunha Cavalacanti 
**Sistema Operacional:** Windows 10/11 
**Navegador:** Google Chrome 

Este documento contém os passos necessários para configurar o host nativo, carregar a extensão no navegador e utilizar a interface de seleção de certificados.

---

## Parte 1: Instalação do Host Nativo e Extensão

Como o Chrome gera um ID único para a extensão ao ser carregada em uma nova máquina, a instalação deve seguir esta ordem específica para que a comunicação Native Messaging funcione corretamente.

### Passo 1: Carregar a extensão no Chrome (Modo Desenvolvedor)
1. Abra o Google Chrome e acesse `chrome://extensions/`.
2. No canto superior direito, ative o **Modo do desenvolvedor**
3. Clique no botão **Carregar sem compactação**
4. Selecione a pasta que contém os arquivos da extensão (`manifest.json`, `popup.html`, `popup.js`, etc.).
5. Após o carregamento, a extensão aparecerá na lista. **Copie o ID gerado** (uma sequência longa de letras).

### Passo 2: Configurar o arquivo de manifesto do Native Messaging
1. Abra o arquivo `nm_manifest.json` em um editor de texto.
2. Na propriedade `"path"`, insira o **caminho absoluto** atualizado para o arquivo `host_nativo.exe` na sua máquina. (Lembre-se de usar barras normais `/` ou barras invertidas duplas `\\`).
3. Na propriedade `"allowed_origins"`, substitua o ID existente pelo ID que você acabou de copiar no Passo 1. O formato deve ficar assim: `"chrome-extension://SEU_NOVO_ID_AQUI/"`.
4. Salve o arquivo.

### Passo 3: Registrar o Host Nativo no Windows 
1. Navegue até a pasta onde o arquivo `install.bat` está localizado.
2. Clique com o botão direito sobre o arquivo `install.bat` e selecione **Executar como Administrador**.
3. Uma janela do prompt de comando será aberta confirmando a criação da chave no registro do Windows. Isso informará ao Chrome o local exato do arquivo `nm_manifest.json`.

---

## Parte 2: Instruções de Uso 

### (a) Abrir a interface 
1. No Google Chrome, clique no ícone de "quebra-cabeça" (Extensões) no canto superior direito da barra de endereços.
2. Clique em **Gerenciador de Certificados TP2** para abrir o *popup* da extensão.

### (b) Visualizar a lista de certificados 
1. Ao abrir, a extensão se comunicará automaticamente com o host nativo em C++.
2. A interface exibirá uma tabela contendo todos os certificados disponíveis no repositório pessoal do usuário (`CurrentUser\My`) do Windows.
3. Você poderá visualizar os metadados de cada certificado, como Assunto (Subject), Emissor (Issuer), Validade, Thumbprint e a indicação se possui chave privada associada.

### (c) Selecionar certificados
1. Utilize os *checkboxes* localizados na primeira coluna da tabela para marcar individualmente os certificados desejados.
2. Para marcar ou desmarcar todos de uma vez, utilize o *checkbox* na linha de cabeçalho da tabela ("Selecionar Todos").

### (d) Salvar e recuperar a seleção
1. Após escolher os certificados, clique no botão **Salvar Seleção** localizado na parte inferior da interface.
2. O botão exibirá a mensagem "Salvo com Sucesso!" indicando que os *thumbprints* foram armazenados localmente (`chrome.storage.local`).
3. Para testar a recuperação da seleção, feche o *popup* clicando em qualquer lugar fora dele.
4. Abra o *popup* novamente. O sistema irá recarregar a lista do Windows e marcará automaticamente os certificados que haviam sido salvos anteriormente.