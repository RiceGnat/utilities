using System;
using System.Runtime.InteropServices;
using System.Threading.Tasks;
using Windows.ApplicationModel;
using Windows.ApplicationModel.Activation;
using Windows.ApplicationModel.VoiceCommands;
using Windows.Foundation;
using Windows.Media.SpeechRecognition;
using Windows.Storage;
using Windows.System;

namespace CortanaCommands
{
	public static class Cortana
	{
		private static async Task WakeMachine(string machine)
		{
			string mac = String.Empty;
			switch (machine)
			{
				case "Kaleina":
					mac = "001bfcf79758"; break;
				case "Kaystros":
					mac = "0023542BE354"; break;
			}
			await Launcher.LaunchUriAsync(new Uri(String.Format("cortana:wol-{0}-192.168.0.255-255.255.255.0", mac)));
		}

		private static async Task ShutdownMachine(string machine)
		{
			await Launcher.LaunchUriAsync(new Uri(String.Format("cortana:shutdown-{0}", machine)));
		}

		private static async Task SuspendMachine(string machine)
		{
			await Launcher.LaunchUriAsync(new Uri(String.Format("cortana:sleep-{0}", machine)));
		}

		private static async Task RestartMachine(string machine)
		{
			await Launcher.LaunchUriAsync(new Uri(String.Format("cortana:restart-{0}", machine)));
		}

		// Register custom Cortana Commands from VCD file
		internal static async void RegisterVCD()
		{
			StorageFile vcd = await Package.Current.InstalledLocation.GetFileAsync(@"VoiceCommandDefinitions.xml");
			await VoiceCommandDefinitionManager.InstallCommandDefinitionsFromStorageFileAsync(vcd);
		}

		// Look up the spoken command and execute its corresponding action
		internal static async Task RunCommand(VoiceCommandActivatedEventArgs e)
		{
			SpeechRecognitionResult result = e.Result;
			string commandName = result.RulePath[0];
			var semanticProperties = result.SemanticInterpretation.Properties;
			string machineName = semanticProperties["machine"][0];

			switch (commandName)
			{
				case "WakeUpComputer":
					await WakeMachine(machineName);
					break;
				case "ShutdownComputer":
					await ShutdownMachine(machineName);
					break;
				case "SuspendComputer":
					await SuspendMachine(machineName);
					break;
				case "RestartComputer":
					await RestartMachine(machineName);
					break;
			}
		}
	}
}
