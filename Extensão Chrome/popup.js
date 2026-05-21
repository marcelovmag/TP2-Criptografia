document.addEventListener('DOMContentLoaded', function() {
    const table = document.getElementById('certTable');
    const tbody = document.getElementById('tableBody');
    const statusDiv = document.getElementById('status');
    const loadingDiv = document.getElementById('loading');
    const btnSave = document.getElementById('btnSave');
    const selectAllCb = document.getElementById('selectAll');

    // Conecta ao Host Nativo
    const port = chrome.runtime.connectNative('com.tp.certlist');

    // Recupera seleções salvas anteriormente
    chrome.storage.local.get(['selectedCerts'], function(result) {
        const savedSelections = result.selectedCerts || [];
        
        // Pede os certificados ao C++
        port.postMessage({ text: "GET_CERTS" });

        port.onMessage.addListener(function(msg) {
            loadingDiv.style.display = 'none';
            table.style.display = 'table';
            tbody.innerHTML = '';
            
            if (msg.length === 0) {
                statusDiv.textContent = "Nenhum certificado encontrado.";
                return;
            }

            statusDiv.textContent = `${msg.length} certificados encontrados.`;

            msg.forEach(function(cert) {
                const tr = document.createElement('tr');

                // Verifica se este certificado já estava salvo
                const isChecked = savedSelections.includes(cert.thumbprint);

                tr.innerHTML = `
                    <td class="col-check">
                        <input type="checkbox" class="cert-check" value="${cert.thumbprint}" ${isChecked ? 'checked' : ''}>
                    </td>
                    <td>${cert.subject}</td>
                    <td>${cert.issuer}</td>
                    <td>${cert.notBefore} a<br>${cert.notAfter}</td>
                    <td class="col-thumb" title="${cert.thumbprint}">${cert.thumbprint.substring(0, 16)}...</td>
                    <td class="${cert.hasKey === true || cert.hasKey === "true" ? 'has-key' : 'no-key'}">
                        ${cert.hasKey === true || cert.hasKey === "true" ? 'SIM' : 'NÃO'}
                    </td>
                `;
                tbody.appendChild(tr);
            });
        });
    });

    // Lógica do "Selecionar Todos"
    selectAllCb.addEventListener('change', function() {
        const checkboxes = document.querySelectorAll('.cert-check');
        checkboxes.forEach(cb => cb.checked = selectAllCb.checked);
    });

    // Lógica de Salvar
    btnSave.addEventListener('click', function() {
        const checkboxes = document.querySelectorAll('.cert-check:checked');
        const selectedValues = Array.from(checkboxes).map(cb => cb.value);

        chrome.storage.local.set({ selectedCerts: selectedValues }, function() {
            const originalText = btnSave.textContent;
            btnSave.textContent = "Salvo com Sucesso!";
            btnSave.style.backgroundColor = "#0d610d"; // Verde
            
            setTimeout(() => {
                btnSave.textContent = originalText;
                btnSave.style.backgroundColor = ""; // Cor original
            }, 2000);
        });
    });

    port.onDisconnect.addListener(function() {
        if (chrome.runtime.lastError) {
            loadingDiv.textContent = "Erro: " + chrome.runtime.lastError.message;
            loadingDiv.style.color = "red";
        }
    });
});