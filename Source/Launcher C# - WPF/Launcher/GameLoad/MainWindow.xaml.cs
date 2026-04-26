using System;
using System.Diagnostics;
using System.IO;
using System.IO.Compression;
using System.Net.Http;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Input;
using System.Windows.Media.Animation;

namespace GameLoad
{
    public partial class MainWindow : Window
    {
        private const string BASE_URL = "https://update.seusite.com/update/";                     //Link de Update
        private const string INI_RELATIVE_PATH = @"Data\GameLoad\Config\Config.ini";              //Pasta que fica no Cliente/Data/GameLoad (criada de forma automática)
        private const string EXE_TO_LAUNCH = "Main.exe";                                          //Executável que o Launcher irá abrir após atualizar
        private const string UPDATE_FILE_PATTERN = @"file_update(\d+)\.zip";                      //Nome do arquivo que irá baixar do cPanel (não mexa)
        private readonly HttpClient _http;
        private string _appBaseDir;
        private string _iniPath;
        private string _exeFullPath;
        private int _localVersion = 0;
        private int _serverLatest = -1;

        private const string LINK_SITE = "https://seusite.com/";                                  //Link do botão Site
        private const string LINK_FORUM = "https://seusite.com/";                                 //Link do botão Fórum
        private const string LINK_NOTICIAS = "https://seusite.com/";                              //Link do botão Noticias
        private const string LINK_VIP = "https://seusite.com/";                                   //Link do botão Vip
        private const string LINK_DONATE = "https://seusite.com/";                                //Link do botão Donate
        private const string LINK_TOOLS = "https://seusite.com/";                                 //Link do botão Tolls (ferramentas ou downloads)

        private const string LINK_FACEBOOK = "https://www.facebook.com";                          //Link do Facebook
        private const string LINK_INSTAGRAM = "https://www.instagram.com";                        //Link do Instagram
        private const string LINK_TIKTOK = "https://whatsapp.com";                                //Link do WhatsApp
        private const string LINK_YOUTUBE = "https://www.youtube.com";                            //Link do Youtube
        private const string LINK_DISCORD = "https://discord.com";                                //Link do Discord

        public MainWindow()
        {
            var handler = new HttpClientHandler { AllowAutoRedirect = true };
            _http = new HttpClient(handler) { Timeout = TimeSpan.FromSeconds(60) };
            _http.DefaultRequestHeaders.UserAgent.ParseAdd("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36");
            InitializeComponent();
            btnClose.Click += (s, e) => Close();
            btnMinimize.Click += (s, e) => WindowState = WindowState.Minimized;
            btnPlay.Click += BtnPlay_Click;

            btnSite.Click += BtnLink_Click;
            btnForum.Click += BtnLink_Click;
            btnNoticias.Click += BtnLink_Click;
            btnVIP.Click += BtnLink_Click;
            btnDonate.Click += BtnLink_Click;
            btnTools.Click += BtnLink_Click;

            btnFacebook.Click += BtnSocial_Click;
            btnInstagram.Click += BtnSocial_Click;
            btnTiktok.Click += BtnSocial_Click;
            btnYoutube.Click += BtnSocial_Click;
            btnDiscord.Click += BtnSocial_Click;

            Loaded += async (s, e) => await StartUpdateProcessAsync();
            btnPlay.IsEnabled = false;
            btnPlay.Opacity = 0.6;
        }

        private void DragArea_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            if (e.ButtonState == MouseButtonState.Pressed)
                DragMove();
        }

        private void BtnLink_Click(object sender, RoutedEventArgs e)
        {
            string url = "";
            if (sender == btnSite) url = LINK_SITE;
            else if (sender == btnForum) url = LINK_FORUM;
            else if (sender == btnNoticias) url = LINK_NOTICIAS;
            else if (sender == btnVIP) url = LINK_VIP;
            else if (sender == btnDonate) url = LINK_DONATE;
            else if (sender == btnTools) url = LINK_TOOLS;
            OpenUrl(url);
            e.Handled = true;
        }

        private void BtnSocial_Click(object sender, RoutedEventArgs e)
        {
            string url = "";
            if (sender == btnFacebook) url = LINK_FACEBOOK;
            else if (sender == btnInstagram) url = LINK_INSTAGRAM;
            else if (sender == btnTiktok) url = LINK_TIKTOK;
            else if (sender == btnYoutube) url = LINK_YOUTUBE;
            else if (sender == btnDiscord) url = LINK_DISCORD;
            OpenUrl(url);
            e.Handled = true;
        }

        private void OpenUrl(string url)
        {
            if (!string.IsNullOrEmpty(url))
            {
                try
                {
                    Process.Start(new ProcessStartInfo { FileName = url, UseShellExecute = true });
                }
                catch { }
            }
        }

        private async Task StartUpdateProcessAsync()
        {
            try
            {
                _appBaseDir = AppDomain.CurrentDomain.BaseDirectory;
                _iniPath = Path.Combine(_appBaseDir, INI_RELATIVE_PATH);
                _exeFullPath = Path.Combine(_appBaseDir, EXE_TO_LAUNCH);
                SetStatus("Lendo versão local...");
                _localVersion = ReadLocalVersion();
                SetStatus("Consultando servidor...");
                _serverLatest = await GetLatestServerVersionAsync();
                txtPercent.Text = "0.00%";
                UpdateProgress(0);
                if (_serverLatest > _localVersion && _serverLatest >= 0)
                {
                    SetStatus($"Atualizando de {_localVersion} para {_serverLatest}");
                    await DownloadAndExtractLatestUpdateAsync(_serverLatest);
                }
                else
                {
                    SetStatus("Jogo atualizado!");
                    await Task.Delay(1500);
                }
                SetStatus("Atualização concluída!");
                txtPercent.Text = "100.00%";
                UpdateProgress(100);
                btnPlay.IsEnabled = true;
                btnPlay.Opacity = 1.0;
                if (File.Exists(_exeFullPath))
                    SetStatus("Clique em JOGAR AGORA para iniciar o jogo");
            }
            catch (Exception ex)
            {
                btnPlay.IsEnabled = false;
                btnPlay.Opacity = 0.6;
                MessageBox.Show($"Falha na atualização:\n{ex.Message}", "Erro", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }

        private void btnConfig_Click(object sender, RoutedEventArgs e)
        {
            var configWindow = new GameLoad.Configs.Configs();
            configWindow.Owner = this;
            this.Hide();
            configWindow.Closed += (s, args) => this.Show();
            configWindow.ShowDialog();
        }

        private void BtnPlay_Click(object sender, RoutedEventArgs e)
        {
            if (File.Exists(_exeFullPath))
            {
                var startInfo = new ProcessStartInfo
                {
                    FileName = _exeFullPath,
                    WorkingDirectory = _appBaseDir,
                    UseShellExecute = true
                };
                Process.Start(startInfo);
                SetStatus("Iniciando Main.exe...");
                //2000 = 2 Mantem o Launcher aberto por 2 Segundos após abrir o Main.exe
                //Necessário para que o sistema do Main de ser executado apenas pelo Launcher funcione sem problemas!
                Task.Delay(2000).ContinueWith(_ =>
                {
                    Dispatcher.Invoke(() => Close());
                });
                return;
            }
        }

        private int ReadLocalVersion()
        {
            if (!File.Exists(_iniPath)) return 0;
            string content = File.ReadAllText(_iniPath);
            var match = Regex.Match(content, @"update_version\s*=\s*(\d+)", RegexOptions.IgnoreCase);
            return match.Success && int.TryParse(match.Groups[1].Value, out int v) ? v : 0;
        }

        private async Task<int> GetLatestServerVersionAsync()
        {
            string html = await _http.GetStringAsync(BASE_URL);
            var regex = new Regex(UPDATE_FILE_PATTERN, RegexOptions.IgnoreCase | RegexOptions.Multiline);
            var matches = regex.Matches(html);
            int highest = -1;
            foreach (Match m in matches)
            {
                if (m.Groups.Count > 1 && int.TryParse(m.Groups[1].Value, out int num))
                    if (num > highest) highest = num;
            }
            return highest;
        }

        private async Task DownloadAndExtractLatestUpdateAsync(int version)
        {
            string zipName = $"file_update{version:D3}.zip";
            string url = BASE_URL.TrimEnd('/') + "/" + zipName;
            string tempZipPath = Path.Combine(_appBaseDir, zipName);
            SetStatus($"Baixando {zipName}...");
            var response = await _http.GetAsync(url, HttpCompletionOption.ResponseHeadersRead);
            response.EnsureSuccessStatusCode();
            long? totalSize = response.Content.Headers.ContentLength;
            long downloaded = 0;
            var sw = System.Diagnostics.Stopwatch.StartNew();
            long lastBytes = 0;
            using (var stream = await response.Content.ReadAsStreamAsync())
            using (var fs = new FileStream(tempZipPath, FileMode.Create, FileAccess.Write))
            {
                byte[] buffer = new byte[81920];
                int read;
                while ((read = await stream.ReadAsync(buffer, 0, buffer.Length)) > 0)
                {
                    await fs.WriteAsync(buffer, 0, read);
                    downloaded += read;
                    if (totalSize.HasValue && totalSize.Value > 0)
                    {
                        double pct = (downloaded * 100.0) / totalSize.Value;
                        double mbps = 0;
                        if (sw.ElapsedMilliseconds > 0)
                            mbps = ((downloaded - lastBytes) / 1024.0 / 1024.0) / (sw.ElapsedMilliseconds / 1000.0);
                        string totalMB = (totalSize.Value / 1024.0 / 1024.0).ToString("F2");
                        string downMB = (downloaded / 1024.0 / 1024.0).ToString("F2");
                        Dispatcher.Invoke(() =>
                        {
                            UpdateProgress((int)pct, false);
                            txtPercent.Text = $"{pct:F2}%";
                            txtSpeed.Text = $"{downMB}MB/{totalMB}MB · {mbps:F2} MB/s";
                        });
                    }
                }
            }
            SetStatus("Extraindo arquivos...");
            txtPercent.Text = "0.00%";
            UpdateProgress(0, false);
            await Task.Run(() =>
            {
                using (var zip = ZipFile.OpenRead(tempZipPath))
                {
                    int total = zip.Entries.Count;
                    int done = 0;
                    foreach (var entry in zip.Entries)
                    {
                        if (string.IsNullOrEmpty(entry.Name)) continue;
                        string destPath = Path.Combine(_appBaseDir, entry.FullName);
                        string directoryPath = Path.GetDirectoryName(destPath);
                        if (!string.IsNullOrEmpty(directoryPath))
                            Directory.CreateDirectory(directoryPath);
                        entry.ExtractToFile(destPath, true);
                        done++;
                        double pct = (done * 100.0) / total;
                        Dispatcher.Invoke(() =>
                        {
                            UpdateProgress((int)pct, false);
                            txtPercent.Text = $"{pct:F2}%";
                            txtSpeed.Text = $"Extraindo {done}/{total} arquivos";
                        });
                    }
                }
            });
            if (File.Exists(tempZipPath))
                File.Delete(tempZipPath);
            UpdateIniVersion(version);
            await Dispatcher.InvokeAsync(() =>
            {
                UpdateProgress(100, true);
                txtPercent.Text = "100.00%";
                txtSpeed.Text = "Extração concluída";
            }, System.Windows.Threading.DispatcherPriority.ApplicationIdle);
        }

        private void UpdateIniVersion(int newVer)
        {
            string dir = Path.GetDirectoryName(_iniPath);
            if (!Directory.Exists(dir)) Directory.CreateDirectory(dir);
            string content = File.Exists(_iniPath) ? File.ReadAllText(_iniPath) : "[Update Config]\r\n";
            var regex = new Regex(@"update_version\s*=\s*\d+", RegexOptions.IgnoreCase);
            if (regex.IsMatch(content))
                content = regex.Replace(content, $"update_version = {newVer}");
            else
                content += $"update_version = {newVer}\r\n";
            File.WriteAllText(_iniPath, content);
        }

        private void UpdateProgress(int percent, bool immediate = false)
        {
            double maxWidth = ProgressBarContainer.ActualWidth;
            if (maxWidth <= 0) return;
            double target = (maxWidth * percent) / 100.0;
            target = Math.Max(0, Math.Min(target, maxWidth));
            if (immediate)
                ProgressIndicator.Width = target;
            else
            {
                var anim = new DoubleAnimation
                {
                    To = target,
                    Duration = TimeSpan.FromMilliseconds(180),
                    EasingFunction = new CubicEase { EasingMode = EasingMode.EaseOut }
                };
                ProgressIndicator.BeginAnimation(WidthProperty, anim);
            }
        }

        private void SetStatus(string text)
        {
            Dispatcher.Invoke(() => txtStatus.Text = text);
        }
    }
}