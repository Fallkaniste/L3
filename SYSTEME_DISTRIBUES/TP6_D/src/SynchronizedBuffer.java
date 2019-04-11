import java.util.ArrayList;

public class SynchronizedBuffer<T>
{
	private ArrayList<T> elements;
	
	public SynchronizedBuffer()
	{
		this.elements=new ArrayList<>();
	}
	
	public synchronized T removeElements(boolean sync)
	{
		if(!sync && this.elements.isEmpty())
		{
			return null;
		}
		while(this.elements.isEmpty())
		{
			try
			{
				this.wait();
			}
			catch(InterruptedException e)
			{
				e.printStackTrace();
			}		
		}
		
		T elt=this.elements.get(0);
		
		try
		{
			this.elements.remove(0);
		}
		catch(ArrayIndexOutOfBoundsException e)
		{
			e.printStackTrace();
		}
		
		return elt;
	}
	
	public synchronized void addElement(T elt)
	{
		this.elements.add(elt);
		this.notifyAll();
	}
	
	public int avalaible()
	{
		return this.elements.size();
	}
}
