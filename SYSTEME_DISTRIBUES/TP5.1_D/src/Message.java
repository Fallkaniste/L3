import java.io.Serializable;

public class Message implements Serializable 
{
	
	private static final long serialVersionUID = -2723363051271966964L;
	private Object value;

	public Message()
	{
		value=null;
	}
	
	public Message(Object o)
	{
		this.value=o;
	}
	
	public Object getValue()
	{
		return this.value;
	}

}
