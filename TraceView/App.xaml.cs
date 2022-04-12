using System.Windows;

namespace TraceView
{
    /// <summary>
    /// Interaktionslogik für "App.xaml"
    /// </summary>
    public partial class App : Application
    {
        private void Application_Startup(object sender, StartupEventArgs e)
        {

            MainWindowViewModel viewModel = new MainWindowViewModel();
            MainWindow mainwindow = new MainWindow(viewModel);
            mainwindow.Show();
        }
    }
}
