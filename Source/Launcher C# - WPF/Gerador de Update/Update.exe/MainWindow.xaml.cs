using System;
using System.IO;
using System.IO.Compression;
using System.Linq;
using System.Text.RegularExpressions;
using System.Windows;
using System.Windows.Input;
using Microsoft.Win32;

namespace Update.exe
{
    public partial class MainWindow : Window
    {
        private string pastaSelecionada;
        private int versaoAtual = 0;
        private string pastaDoPrograma;

        public MainWindow()
        {
            InitializeComponent();
            pastaDoPrograma = AppDomain.CurrentDomain.BaseDirectory;
            CarregarVersaoInicial();
        }

        private void CarregarVersaoInicial()
        {
            string configPath = Path.Combine(pastaDoPrograma, "Data", "Config.ini");

            if (File.Exists(configPath))
            {
                string conteudo = File.ReadAllText(configPath);
                var match = Regex.Match(conteudo, @"update_version\s*=\s*(\d+)", RegexOptions.IgnoreCase);

                if (match.Success && int.TryParse(match.Groups[1].Value, out int versao))
                {
                    versaoAtual = versao;
                }
            }

            txtVersaoAtual.Text = versaoAtual.ToString();
            txtNovaVersao.Text = (versaoAtual + 1).ToString("D3");
        }

        private void DragArea_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            this.DragMove();
        }

        private void btnClose_Click(object sender, RoutedEventArgs e)
        {
            Close();
        }

        private void btnProcurar_Click(object sender, RoutedEventArgs e)
        {
            var dialog = new OpenFileDialog
            {
                Title = "Selecione a pasta Files",
                ValidateNames = false,
                CheckFileExists = false,
                CheckPathExists = true,
                FileName = "Selecione esta pasta",
                InitialDirectory = pastaDoPrograma
            };

            if (dialog.ShowDialog() == true)
            {
                pastaSelecionada = Path.GetDirectoryName(dialog.FileName);

                if (Path.GetFileName(pastaSelecionada).ToLower() != "files")
                {
                    MessageBox.Show("Por favor, selecione a pasta 'Files'", "Aviso",
                        MessageBoxButton.OK, MessageBoxImage.Warning);
                    return;
                }

                txtPastaSelecionada.Text = pastaSelecionada;
                txtPastaSelecionada.Foreground = System.Windows.Media.Brushes.White;
                txtPastaSelecionada.FontStyle = FontStyles.Normal;
                CarregarArquivos();
                btnGerar.IsEnabled = true;
            }
        }

        private void CarregarArquivos()
        {
            lstArquivos.Items.Clear();

            if (!Directory.Exists(pastaSelecionada))
                return;

            var arquivos = Directory.GetFiles(pastaSelecionada, "*.*", SearchOption.AllDirectories)
                                     .Select(f => f.Substring(pastaSelecionada.Length + 1))
                                     .Where(f => !f.Equals("Data\\Config.ini", StringComparison.OrdinalIgnoreCase) &&
                                                !f.Equals("Config.ini", StringComparison.OrdinalIgnoreCase))
                                     .OrderBy(f => f)
                                     .ToList();

            foreach (var arquivo in arquivos)
            {
                lstArquivos.Items.Add(arquivo);
            }

            txtPastaSelecionada.Text = $"{pastaSelecionada}  |  📄 {arquivos.Count} arquivo(s) encontrado(s)";
        }

        private void btnGerar_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                int novaVersao = versaoAtual + 1;
                string nomeArquivo = $"file_update{novaVersao:D3}.zip";

                string pastaUpdate = Path.Combine(pastaDoPrograma, "Update");
                if (!Directory.Exists(pastaUpdate))
                {
                    Directory.CreateDirectory(pastaUpdate);
                }

                string caminhoZip = Path.Combine(pastaUpdate, nomeArquivo);

                string tempZip = Path.Combine(Path.GetTempPath(), nomeArquivo);
                if (File.Exists(tempZip))
                    File.Delete(tempZip);

                using (FileStream zipStream = new FileStream(tempZip, FileMode.Create))
                using (ZipArchive archive = new ZipArchive(zipStream, ZipArchiveMode.Create))
                {
                    var todosArquivos = Directory.GetFiles(pastaSelecionada, "*.*", SearchOption.AllDirectories);

                    foreach (string arquivo in todosArquivos)
                    {
                        string entryName = arquivo.Substring(pastaSelecionada.Length + 1);
                        archive.CreateEntryFromFile(arquivo, entryName, CompressionLevel.Optimal);
                    }
                }

                if (File.Exists(caminhoZip))
                    File.Delete(caminhoZip);
                File.Move(tempZip, caminhoZip);

                string configPath = Path.Combine(pastaDoPrograma, "Data", "Config.ini");
                string pastaConfig = Path.GetDirectoryName(configPath);

                if (!Directory.Exists(pastaConfig))
                {
                    Directory.CreateDirectory(pastaConfig);
                }

                string conteudo;

                if (File.Exists(configPath))
                {
                    conteudo = File.ReadAllText(configPath);
                    var regex = new Regex(@"update_version\s*=\s*\d+", RegexOptions.IgnoreCase);

                    if (regex.IsMatch(conteudo))
                    {
                        conteudo = regex.Replace(conteudo, $"update_version = {novaVersao}");
                    }
                    else
                    {
                        conteudo += $"\r\nupdate_version = {novaVersao}";
                    }
                }
                else
                {
                    conteudo = $"[Config]\r\nupdate_version = {novaVersao}";
                }

                File.WriteAllText(configPath, conteudo);

                MessageBox.Show(
                    $"✅ Update gerado com sucesso!\n\n" +
                    $"📦 Arquivo: {nomeArquivo}\n" +
                    $"📍 Local: {caminhoZip}\n" +
                    $"📌 Versão atualizada para: {novaVersao}\n\n" +
                    $"📄 Total de arquivos compactados: {lstArquivos.Items.Count}",
                    "Sucesso",
                    MessageBoxButton.OK,
                    MessageBoxImage.Information);

                versaoAtual = novaVersao;
                txtVersaoAtual.Text = versaoAtual.ToString();
                txtNovaVersao.Text = (versaoAtual + 1).ToString("D3");
            }
            catch (Exception ex)
            {
                MessageBox.Show($"❌ Erro ao gerar update:\n\n{ex.Message}",
                    "Erro", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }
    }
}