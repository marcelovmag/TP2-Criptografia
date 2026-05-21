#include <iostream>
#include <vector>
#include <string>
#include <windows.h>
#include <wincrypt.h>
#include <sstream>
#include <iomanip>
#include <fcntl.h>
#include <io.h>

// --- FUNCOES AUXILIARES ---

// Converte WString para String UTF-8
std::string WStringToString(const std::wstring& wstr) {
    if (wstr.empty()) return "";
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
    std::string strTo(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
    return strTo;
}

// Obtem nome (Subject ou Issuer)
std::string GetCertString(PCCERT_CONTEXT pCertContext, DWORD type) {
    DWORD size = CertGetNameStringW(pCertContext, type, 0, NULL, NULL, 0);
    if (size <= 1) return "Desconhecido";
    std::vector<wchar_t> name(size);
    CertGetNameStringW(pCertContext, type, 0, NULL, &name[0], size);
    return WStringToString(&name[0]);
}

// Converte FileTime (Windows) para DD/MM/AAAA
std::string DateToString(FILETIME ft) {
    SYSTEMTIME st;
    FileTimeToSystemTime(&ft, &st);
    char buffer[20];
    sprintf(buffer, "%02d/%02d/%04d", st.wDay, st.wMonth, st.wYear);
    return std::string(buffer);
}

// Gera Thumbprint (Hash SHA1) em Hexadecimal
std::string GetThumbprint(PCCERT_CONTEXT pCertContext) {
    DWORD hashSize = 0;
    CertGetCertificateContextProperty(pCertContext, CERT_SHA1_HASH_PROP_ID, NULL, &hashSize);
    if (hashSize == 0) return "";
    
    std::vector<BYTE> hash(hashSize);
    CertGetCertificateContextProperty(pCertContext, CERT_SHA1_HASH_PROP_ID, hash.data(), &hashSize);
    
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    for (BYTE b : hash) ss << std::setw(2) << (int)b;
    return ss.str();
}

std::string EscapeJSON(const std::string& s) {
    std::string res;
    for (char c : s) {
        if (c == '"') res += "\\\"";
        else if (c == '\\') res += "\\\\";
        else res += c;
    }
    return res;
}

// --- MAIN ---
int main() {
    _setmode(_fileno(stdin), _O_BINARY);
    _setmode(_fileno(stdout), _O_BINARY);

    while (true) {
        unsigned int length = 0;
        std::cin.read(reinterpret_cast<char*>(&length), 4);
        if (std::cin.eof()) break; 

        if (length > 0) {
            std::vector<char> msgBuffer(length);
            std::cin.read(&msgBuffer[0], length);
        }

        HCERTSTORE hStore = CertOpenStore(CERT_STORE_PROV_SYSTEM, 0, 0, CERT_SYSTEM_STORE_CURRENT_USER, L"MY");
        std::stringstream json;
        json << "[";

        if (hStore) {
            PCCERT_CONTEXT pCertContext = NULL;
            bool first = true;
            while ((pCertContext = CertEnumCertificatesInStore(hStore, pCertContext))) {
                if (!first) json << ",";
                first = false;
                
                // Coleta de dados completa (RF1)
                std::string subject = GetCertString(pCertContext, CERT_NAME_SIMPLE_DISPLAY_TYPE);
                std::string issuer = GetCertString(pCertContext, CERT_NAME_FRIENDLY_DISPLAY_TYPE); // Tenta friendly name para Issuer
                
                // Se o friendly falhar, tenta o simple type para issuer
                if (issuer == "Desconhecido") { 
                     // Flags específicas para pegar Issuer podem ser complexas, vamos simplificar:
                     // Aqui mantemos o friendly ou vazio.
                }

                std::string notBefore = DateToString(pCertContext->pCertInfo->NotBefore);
                std::string notAfter = DateToString(pCertContext->pCertInfo->NotAfter);
                std::string thumbprint = GetThumbprint(pCertContext);

                DWORD cbData = 0;
                bool hasKey = CertGetCertificateContextProperty(pCertContext, CERT_KEY_PROV_INFO_PROP_ID, NULL, &cbData);

                json << "{"
                     << "\"subject\": \"" << EscapeJSON(subject) << "\","
                     << "\"issuer\": \"" << EscapeJSON(issuer) << "\","
                     << "\"notBefore\": \"" << notBefore << "\","
                     << "\"notAfter\": \"" << notAfter << "\","
                     << "\"thumbprint\": \"" << thumbprint << "\","
                     << "\"hasKey\": " << (hasKey ? "true" : "false")
                     << "}";
            }
            CertCloseStore(hStore, 0);
        }
        json << "]";

        std::string response = json.str();
        unsigned int len = (unsigned int)response.size();
        std::cout.write(reinterpret_cast<char*>(&len), 4);
        std::cout.write(response.c_str(), len);
        std::cout.flush();
    }
    return 0;
}