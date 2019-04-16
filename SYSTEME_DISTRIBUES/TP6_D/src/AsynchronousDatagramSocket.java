import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.SocketException;

public class AsynchronousDatagramSocket 
{
	private DatagramSocket socket;
	private SynchronizedBuffer<DatagramPacket> buffer;
	
	public AsynchronousDatagramSocket() throws SocketException
	{
		socket=new DatagramSocket(0);
		buffer=new SynchronizedBuffer<>();
		(new SocketReader(socket, buffer)).start();
	}
	
	public AsynchronousDatagramSocket(int port) throws SocketException
	{
		socket=new DatagramSocket(port);
		buffer=new SynchronizedBuffer<>();
		(new SocketReader(socket, buffer)).start();
	}
	
	public void send(DatagramPacket dp) throws IOException
	{
		socket.send(dp);
	}
	
	public DatagramPacket asynchronousReceive()
	{
		DatagramPacket dp = buffer.removeElements(false);
		return dp;
	}
	
	public DatagramPacket synchronousReceive()
	{
		DatagramPacket dp = buffer.removeElements(true);
		return dp;
	}
	
	public void close()
	{
		this.socket.close();
	}
	
	public boolean available()
	{
		return buffer.avalaible()>=1;
	}
	
	private class SocketReader extends Thread
	{
		private DatagramSocket socket;
		private SynchronizedBuffer<DatagramPacket> buffer;
		
		public SocketReader(DatagramSocket socket, SynchronizedBuffer<DatagramPacket> buffer)
		{
			this.socket=socket;
			this.buffer=buffer;
		}
		
		public void run()
		{
			byte[] tab;
			DatagramPacket dp;
			
			try
			{
				while(!socket.isClosed())
				{
					sleep(500);
					tab=new byte[60000];
					dp=new DatagramPacket(tab,tab.length);
					if(!socket.isClosed())
					{
						socket.receive(dp);
						buffer.addElement(dp);
					}
				}
			}
			catch(Exception e)
			{
				System.out.println(socket.isClosed());
				e.printStackTrace();
			}
		}
	}
}
