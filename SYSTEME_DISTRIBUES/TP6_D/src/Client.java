import java.io.IOException;
import java.net.DatagramPacket;
import java.net.InetAddress;

public class Client 
{

	public static void main(String[] args) 
	{
		String hostName="localhost";
		int port=7777;
		
		if (args.length!=2)
		{
			System.out.println("Vous devez entrer uniquement le nom d'hôte et le port (dans cet ordre).");
			System.exit(-1);
		}
		else
		{
			hostName=args[0];
			port=Integer.parseInt(args[1]);
		}
		
		try 
		{
			// adresse IP du serveur
			InetAddress adr = InetAddress.getByName(hostName);
		
			AsynchronousDatagramSocket socket =new AsynchronousDatagramSocket();

			byte[] a =new byte[1];
			a[0]='z';
			socket.send(new DatagramPacket(a,a.length, adr, port));
			socket.close();
			
		} 
		catch (IOException e) 
		{
			e.printStackTrace();
		}
		
	}

}
