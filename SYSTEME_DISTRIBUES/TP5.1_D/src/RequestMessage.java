import java.io.Serializable;

public class RequestMessage implements Serializable 
{
	private static final long serialVersionUID = 2558096676323121907L;
	private String metName;
	@SuppressWarnings("rawtypes")
	private Class[] paramTypes;
	private Object[] param;
	
	public RequestMessage()
	{
		metName=null;
		paramTypes=null;
		param=null;
	}
	
	public RequestMessage(String metName, Class<Object>[] paramTypes, Object[] param)
	{
		this.metName=metName;
		this.paramTypes=paramTypes;
		this.param=param;
	}
	
	public String getMethod()
	{
		return this.metName;
	}
	
	@SuppressWarnings("rawtypes")
	public Class[] getParametersTypes()
	{
		return this.paramTypes;
	}
	
	public Object[] getParameters()
	{
		return this.param;
	}
}
