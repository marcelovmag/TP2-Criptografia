@echo off
:: Este comando pega o caminho da pasta onde este arquivo esta salvo
SET "PASTA_ATUAL=%~dp0"
:: Remove a barra invertida final, se houver, para evitar duplicação
IF %PASTA_ATUAL:~-1%==\ SET PASTA_ATUAL=%PASTA_ATUAL:~0,-1%

:: Define o caminho do manifesto (json)
SET "CAMINHO_MANIFEST=%PASTA_ATUAL%\nm_manifest.json"

echo Registrando o Host Nativo no Windows...
echo Caminho detectado: %CAMINHO_MANIFEST%

:: Cria a chave no registro do Windows
REG ADD "HKCU\Software\Google\Chrome\NativeMessagingHosts\com.tp.certlist" /ve /t REG_SZ /d "%CAMINHO_MANIFEST%" /f

echo.
if %errorlevel% equ 0 (
    echo ==========================================
    echo SUCESSO! O registro foi criado.
    echo Agora o Chrome sabe onde encontrar seu programa.
    echo ==========================================
) else (
    echo ==========================================
    echo ERRO! Falha ao criar o registro.
    echo Tente clicar com botao direito e "Executar como Administrador".
    echo ==========================================
)
pause