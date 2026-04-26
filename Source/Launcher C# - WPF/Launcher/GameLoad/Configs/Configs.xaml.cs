using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Windows;
using System.Windows.Input;
using Microsoft.Win32;

namespace GameLoad.Configs
{
    public partial class Configs : Window
    {
        private readonly string configPath = System.IO.Path.Combine(
            AppDomain.CurrentDomain.BaseDirectory,
            "Data", "Custom", "config.ini"
        );
        private List<string> fileLines = new List<string>();

        private void SaveFpsInstantly()
        {
            try
            {
                if (!File.Exists(configPath))
                    return;
                fileLines = File.ReadAllLines(configPath).ToList();
                int fpsReal = (int)sldFpsLimit.Value;
                if (fpsReal > 64) fpsReal = 64;
                SetValue("FpsLimit", fpsReal.ToString());
                File.WriteAllLines(configPath, fileLines);
            }
            catch { }
        }

        public Configs()
        {
            InitializeComponent();
            cmbResolution.PreviewMouseLeftButtonDown += (s, e) =>
            {
                if (!cmbResolution.IsDropDownOpen)
                {
                    cmbResolution.IsDropDownOpen = true;
                    e.Handled = true;
                }
            };
            sldFpsLimit.ValueChanged += (s, e) =>
            {
                txtFpsValue.Text = $"{sldFpsLimit.Value:0} FPS";
                SaveFpsInstantly();
            };
            cmbResolution.SelectedIndex = 8;
            LoadConfig();
        }

        private void LoadConfig()
        {
            if (!File.Exists(configPath)) return;
            try
            {
                fileLines = File.ReadAllLines(configPath).ToList();
                txtLogin.Text = GetValue("User");
                txtPassword.Password = GetValue("Password");
                chkAutoLogin.IsChecked = GetValue("AutoLoginEnable") == "1";
                double fpsValue = ToDouble(GetValue("FpsLimit"), 60);
                if (fpsValue > 64) fpsValue = 64;
                sldFpsLimit.Value = fpsValue;
                chkOpenGL.IsChecked = GetValue("UseOpenGL") == "1";
                chkHideEffect.IsChecked = GetValue("EffectSprite") == "1";
                chkHideParticle.IsChecked = GetValue("EffectParticle") == "1";
                chkHideTerrain.IsChecked = GetValue("Terrain") == "1";
                chkHideWings.IsChecked = GetValue("Wings") == "1";
                chkHideSwords.IsChecked = GetValue("Swords") == "1";
                chkHideSets.IsChecked = GetValue("Equipments") == "1";
                chkHideObjects.IsChecked = GetValue("Object") == "1";
                chkHideShadows.IsChecked = GetValue("Shadows") == "1";
                chkHideMonsters.IsChecked = GetValue("Monsters") == "1";
                chkHidePlayers.IsChecked = GetValue("Players") == "1";
                chkHideNewSkill.IsChecked = GetValue("NewSkill") == "1";
                chkVisualWings.IsChecked = GetValue("WingsLv2") == "1";
                chkMiniMounts.IsChecked = GetValue("MiniMounts") == "1";
                chkHideInterface.IsChecked = GetValue("HideInterface") == "1";
                chkSlideHelp.IsChecked = GetValue("SlideHelp") == "1";
                chkAutoAttack.IsChecked = GetValue("AutoAttack") == "1";
                chkWhisperSound.IsChecked = GetValue("WhisperSound") == "1";
                LoadRegistrySettings();
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Erro ao carregar configurações: {ex.Message}",
                              "Erro", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }

        private void LoadRegistrySettings()
        {
            try
            {
                using (RegistryKey key = Registry.CurrentUser.OpenSubKey(@"Software\MuOnline\Config"))
                {
                    if (key != null)
                    {
                        int soundValue = (int)(key.GetValue("SoundOnOFF") ?? 1);
                        chkSound.IsChecked = soundValue == 1;
                        int musicValue = (int)(key.GetValue("MusicOnOFF") ?? 1);
                        chkMusic.IsChecked = musicValue == 1;
                        int windowModeValue = (int)(key.GetValue("WindowMode") ?? 0);
                        chkWindowed.IsChecked = windowModeValue == 1;
                        int resolutionValue = (int)(key.GetValue("Resolution") ?? 9);
                        if (resolutionValue >= 1 && resolutionValue <= 10)
                        {
                            cmbResolution.SelectedIndex = resolutionValue - 1;
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Erro ao ler registro: {ex.Message}");
            }
        }

        private string GetValue(string key)
        {
            var line = fileLines.FirstOrDefault(l => l.Trim().StartsWith(key + "="));
            if (line == null) return "";
            var parts = line.Split('=');
            return parts.Length > 1 ? parts[1].Trim() : "";
        }

        private double ToDouble(string val, double def)
        {
            return double.TryParse(val, out double r) ? r : def;
        }

        private void ApplyRegistrySettings()
        {
            try
            {
                using (RegistryKey key = Registry.CurrentUser.CreateSubKey(@"Software\MuOnline\Config"))
                {
                    if (key != null)
                    {
                        int resolutionIndex = cmbResolution.SelectedIndex + 1;
                        key.SetValue("Resolution", resolutionIndex, RegistryValueKind.DWord);
                        int windowModeValue = chkWindowed.IsChecked == true ? 1 : 0;
                        key.SetValue("WindowMode", windowModeValue, RegistryValueKind.DWord);
                        int soundValue = chkSound.IsChecked == true ? 1 : 0;
                        key.SetValue("SoundOnOFF", soundValue, RegistryValueKind.DWord);
                        int volumeLevel = chkSound.IsChecked == true ? 3 : 0;
                        key.SetValue("VolumeLevel", volumeLevel, RegistryValueKind.DWord);
                        int musicValue = chkMusic.IsChecked == true ? 1 : 0;
                        key.SetValue("MusicOnOFF", musicValue, RegistryValueKind.DWord);
                    }
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Erro ao salvar configurações no registro: {ex.Message}",
                               "Erro", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }

        private void ApplyOpenGLFiles(bool enable)
        {
            try
            {
                string baseDir = AppDomain.CurrentDomain.BaseDirectory;
                string sourceDir = Path.Combine(baseDir, "Data", "Custom", "OpenGL");
                string destDir = baseDir;
                string[] files = { "OpenGL32.dll", "Settings.ini" };
                if (enable)
                {
                    if (!Directory.Exists(sourceDir))
                    {
                        MessageBox.Show("Pasta OpenGL não encontrada em: " + sourceDir,
                                       "Aviso", MessageBoxButton.OK, MessageBoxImage.Warning);
                        return;
                    }
                    foreach (string file in files)
                    {
                        string sourceFile = Path.Combine(sourceDir, file);
                        string destFile = Path.Combine(destDir, file);
                        if (File.Exists(sourceFile))
                        {
                            File.Copy(sourceFile, destFile, true);
                        }
                    }
                }
                else
                {
                    foreach (string file in files)
                    {
                        string destFile = Path.Combine(destDir, file);
                        if (File.Exists(destFile))
                        {
                            File.Delete(destFile);
                        }
                    }
                }
            }
            catch (Exception ex)
            {
            }
        }

        private void btnSave_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                string directory = Path.GetDirectoryName(configPath);
                if (!Directory.Exists(directory))
                {
                    Directory.CreateDirectory(directory);
                }
                if (!File.Exists(configPath))
                {
                    CreateDefaultConfigFile();
                }
                fileLines = File.ReadAllLines(configPath).ToList();
                SetValue("User", txtLogin.Text);
                SetValue("Password", txtPassword.Password);
                SetValue("AutoLoginEnable", chkAutoLogin.IsChecked == true ? "1" : "0");
                int fpsReal = (int)sldFpsLimit.Value;
                if (fpsReal > 64) fpsReal = 64;
                SetValue("FpsLimit", fpsReal.ToString());
                SetValue("UseOpenGL", chkOpenGL.IsChecked == true ? "1" : "0");
                SetValue("EffectSprite", chkHideEffect.IsChecked == true ? "1" : "0");
                SetValue("EffectParticle", chkHideParticle.IsChecked == true ? "1" : "0");
                SetValue("Terrain", chkHideTerrain.IsChecked == true ? "1" : "0");
                SetValue("Wings", chkHideWings.IsChecked == true ? "1" : "0");
                SetValue("Swords", chkHideSwords.IsChecked == true ? "1" : "0");
                SetValue("Equipments", chkHideSets.IsChecked == true ? "1" : "0");
                SetValue("Object", chkHideObjects.IsChecked == true ? "1" : "0");
                SetValue("Shadows", chkHideShadows.IsChecked == true ? "1" : "0");
                SetValue("Monsters", chkHideMonsters.IsChecked == true ? "1" : "0");
                SetValue("Players", chkHidePlayers.IsChecked == true ? "1" : "0");
                SetValue("NewSkill", chkHideNewSkill.IsChecked == true ? "1" : "0");
                SetValue("WingsLv2", chkVisualWings.IsChecked == true ? "1" : "0");
                SetValue("MiniMounts", chkMiniMounts.IsChecked == true ? "1" : "0");
                SetValue("HideInterface", chkHideInterface.IsChecked == true ? "1" : "0");
                SetValue("SlideHelp", chkSlideHelp.IsChecked == true ? "1" : "0");
                SetValue("AutoAttack", chkAutoAttack.IsChecked == true ? "1" : "0");
                SetValue("WhisperSound", chkWhisperSound.IsChecked == true ? "1" : "0");
                File.WriteAllLines(configPath, fileLines);
                ApplyRegistrySettings();
                ApplyOpenGLFiles(chkOpenGL.IsChecked == true);
                Close();
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Erro ao salvar configurações: {ex.Message}", "Erro", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }

        private void CreateDefaultConfigFile()
        {
            fileLines = new List<string>();
        }

        private void SetValue(string key, string value)
        {
            bool found = false;
            for (int i = 0; i < fileLines.Count; i++)
            {
                if (fileLines[i].Trim().StartsWith(key + "="))
                {
                    fileLines[i] = $"{key}={value}";
                    found = true;
                    return;
                }
            }
            if (!found)
            {
                for (int i = 0; i < fileLines.Count; i++)
                {
                    if (fileLines[i].Trim() == "[AntiLagOptions]")
                    {
                        fileLines.Insert(i + 1, $"{key}={value}");
                        return;
                    }
                }
            }
        }

        private void DragArea_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            if (e.ButtonState == MouseButtonState.Pressed)
                DragMove();
        }

        private void btnClose_Click(object sender, RoutedEventArgs e)
        {
            Close();
        }
    }
}