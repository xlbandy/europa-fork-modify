package psengine.util;
import java.text.NumberFormat;

/**
 * SimpleTimer calculates Elapsed (real) time
 */
public class SimpleTimer 
{
  private long    startTime_; 
  private long 	  totalTime_;
  private boolean running_;

  public SimpleTimer ()
  {
	reset();
  }

  public SimpleTimer reset()
  {
	  startTime_ = 0;
	  totalTime_ = 0;    
	  running_ = false;
	  return this;
  }
  
  public SimpleTimer start ()
  {
	  running_ = true;
    startTime_ = System.currentTimeMillis();
    return this;
  }


  public SimpleTimer stop ()
  {
    if (running_)
    { 
    	updateTotal();
      running_ = false;
    }
    return this;
  }
  
  /**
   * @return ElapsedTime in miliseconds 
   */
  public long getElapsed ()
  {
    if (running_)
    {
    	updateTotal();
    }
    return totalTime_;
  }

  /**
   * @return ElapsedTime string, nicely formatted
   */
  public String getElapsedString () {
    return format(getElapsed());
  }

  /**
   * @parm divisor what to divide the elapsed time by
   * @return ElapsedTime string divided by divisor
   */
  public String getElapsedPer (int divisor) {
    if (divisor == 0)
      return "Infinity";
    long elapsed = getElapsed();
    double dElapsed = elapsed*1.0 / divisor;
    if (dElapsed < 1000)
      return decimal3.format(dElapsed) + " msecs";
    return format(elapsed);
  }

  /**
   * Format a millisecond elapsed time into a string
   */
  public static String format (long msecs) {
    if (msecs < 1000)
      return Long.toString(msecs) + " msecs";
    else if (msecs < (60*1000))
      return decimal3.format(msecs / 1000.0) + " secs";
    else
      return decimal3.format(msecs / (60*1000.0)) + " mins";
  }

  private void updateTotal()
  {
	  long snapshot = System.currentTimeMillis();
	  totalTime_ +=  snapshot - startTime_;
	  startTime_ = snapshot;
  }
  
  private final static NumberFormat decimal3 = NumberFormat.getNumberInstance();
  {
    decimal3.setMaximumFractionDigits(3);
  }
}
