#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <iomanip>
#include <sstream>
#include <cstdlib>
#include <windows.h>

namespace fs = std::filesystem;

std::string formatVersion(int version) {
    std::ostringstream oss;
    oss << std::setw(3) << std::setfill('0') << version;
    return oss.str();
}

bool createConfigFile(const std::string& basePath, int version) {
    std::string configDir = basePath + "\\Data\\GameLoad\\Config";
    std::string configFile = configDir + "\\Config.ini";

    try {
        fs::create_directories(configDir);
    }
    catch (const std::exception& e) {
        std::cerr << "Erro ao criar diretorios: " << e.what() << std::endl;
        return false;
    }

    if (fs::exists(configFile)) {
        fs::remove(configFile);
    }

    std::ofstream ini(configFile);
    if (!ini.is_open()) {
        std::cerr << "Erro ao criar Config.ini" << std::endl;
        return false;
    }

    ini << "[Update Config]\n";
    ini << "update_version = " << version << "\n";
    ini.close();

    std::cout << "Config.ini criado com sucesso!\n";
    return true;
}

bool zipFolder(const std::string& sourceFolder, const std::string& outputZip) {
    if (!fs::exists(sourceFolder)) {
        std::cerr << "Pasta de origem nao encontrada: " << sourceFolder << std::endl;
        return false;
    }

    std::string outputDir = outputZip.substr(0, outputZip.find_last_of("\\/"));
    try {
        fs::create_directories(outputDir);
    }
    catch (const std::exception& e) {
        std::cerr << "Erro ao criar pasta de destino: " << e.what() << std::endl;
        return false;
    }

    if (fs::exists(outputZip)) {
        fs::remove(outputZip);
    }

    std::string sourceAbs = fs::absolute(sourceFolder).string();
    std::string outputAbs = fs::absolute(outputZip).string();

    std::string cmd = "tar -a -c -f \"" + outputAbs + "\" -C \"" + sourceAbs + "\" *";

    std::cout << "Compactando arquivos...\n";
    int result = system(cmd.c_str());

    if (result != 0) {
        std::cerr << "Erro ao compactar os arquivos (codigo: " << result << ")\n";
        return false;
    }
    return true;
}

void updateClientFiles() {
    int version = 0;
    std::cout << "\nDigite a versao do novo update e aperte enter: ";
    std::cin >> version;

    if (version < 0 || version > 999) {
        std::cerr << "Versao invalida! Use um numero entre 0 e 999.\n";
        return;
    }

    std::string versionStr = formatVersion(version);
    std::string sourcePath = "Files";
    std::string zipName = "file_update" + versionStr + ".zip";
    std::string outputZip = "Update\\Files\\" + zipName;

    if (!fs::exists(sourcePath)) {
        std::cerr << "\nERRO: Pasta 'Files' nao encontrada!\n";
        std::cerr << "Coloque a pasta 'Files' no mesmo diretorio do MainUpdater.exe\n";
        return;
    }

    std::cout << "Gerando " << zipName << "...\n";
    if (zipFolder(sourcePath, outputZip)) {
        std::cout << "\n=== SUCESSO (CLIENT FILES) ===\n";
        std::cout << "Arquivo criado: " << zipName << "\n";
        std::cout << "Caminho: " << fs::absolute(outputZip).string() << "\n\n";
    }
    else {
        std::cerr << "\nFalha ao gerar o arquivo ZIP.\n";
    }
}

void updateMainExe() {
    int version = 0;
    std::cout << "\nDigite a versao do novo update do Main.exe e aperte enter: ";
    std::cin >> version;

    if (version < 0 || version > 999) {
        std::cerr << "Versao invalida! Use um numero entre 0 e 999.\n";
        return;
    }

    std::string versionStr = formatVersion(version);
    std::string sourcePath = "Main";
    std::string zipName = "exe_update" + versionStr + ".zip";
    std::string outputZip = "Update\\Main\\" + zipName;

    if (!fs::exists(sourcePath)) {
        std::cerr << "\nERRO: Pasta 'Main' nao encontrada!\n";
        std::cerr << "Coloque a pasta 'Main' no mesmo diretorio do MainUpdater.exe\n";
        return;
    }

    std::cout << "\nCriando arquivo de configuracao...\n";
    if (!createConfigFile(sourcePath, version)) {
        std::cerr << "Falha ao criar Config.ini. Operacao cancelada.\n";
        return;
    }

    std::cout << "Gerando " << zipName << "...\n";
    if (zipFolder(sourcePath, outputZip)) {
        std::cout << "\n=== SUCESSO (MAIN UPDATE) ===\n";
        std::cout << "Arquivo criado: " << zipName << "\n";
        std::cout << "Caminho: " << fs::absolute(outputZip).string() << "\n\n";
    }
    else {
        std::cerr << "\nFalha ao gerar o arquivo ZIP.\n";
    }
}

int main() {
    SetConsoleTitle(TEXT("MainUpdater"));
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    std::cout << "==========================================\n";
    std::cout << "              MAIN UPDATER\n";
    std::cout << "==========================================\n\n";

    std::cout << "Escolha uma das opcoes abaixo!\n\n";
    std::cout << "1 - Update de Arquivos do Cliente\n";
    std::cout << "2 - Update do Main.exe\n\n";
    std::cout << "Opcao: ";

    int opcao = 0;
    std::cin >> opcao;

    switch (opcao) {
    case 1:
        updateClientFiles();
        break;
    case 2:
        updateMainExe();
        break;
    default:
        std::cerr << "\nOpcao invalida!\n";
        break;
    }

    std::cout << "\nPressione qualquer tecla para sair...\n";
    system("pause > nul");
    return 0;
}