package com.studio.artaban.apptest;

import java.io.IOException;
import java.net.HttpURLConnection;
import java.net.InetAddress;
import java.net.MalformedURLException;
import java.net.NetworkInterface;
import java.net.SocketTimeoutException;
import java.net.URL;
import java.net.UnknownHostException;
import java.util.Collections;
import java.util.List;

import org.apache.http.conn.util.InetAddressUtils;

import android.content.Context;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.util.Log;

import com.studio.artaban.apptest.data.EngData;

public class EngNetworks {

    private Context mContext;
    public EngNetworks(Context content) { mContext = content; }    

    ////// Internet
    static private final short CONNECTION_TYPE_NONE = 0;
    static private final short CONNECTION_TYPE_WIFI = 1;
    static private final short CONNECTION_TYPE_UNKNOWN = 2; // Other

    public short isConnected() {

    	if (!EngData.USES_PERMISSION_INTERNET) {
    		Log.wtf("EngNetworks", "No USES_PERMISSION_INTERNET permission flag");
    		return EngNetworks.CONNECTION_TYPE_NONE;
    	}
    	ConnectivityManager connManager = ((ConnectivityManager)mContext.getSystemService(Context.CONNECTIVITY_SERVICE));
    	NetworkInfo netInfo = connManager.getActiveNetworkInfo();
    	if ((netInfo != null) && (netInfo.isConnected())) {

    		NetworkInfo wifiConn = connManager.getNetworkInfo(ConnectivityManager.TYPE_WIFI);
    		if ((wifiConn != null) && (wifiConn.isConnected()))
    			return CONNECTION_TYPE_WIFI;
    		return CONNECTION_TYPE_UNKNOWN;
    	}
		return CONNECTION_TYPE_NONE;
    }

    static public boolean isOnline(int timeOut) {

    	if (!EngData.USES_PERMISSION_INTERNET) {
    		Log.wtf("EngNetworks", "No USES_PERMISSION_INTERNET permission flag");
    		return false;
    	}
    	try {

            URL url = new URL("http://www.google.com"); // URL to check
            HttpURLConnection connURL = (HttpURLConnection)url.openConnection();
            connURL.setConnectTimeout(timeOut);
            connURL.connect();
            if (connURL.getResponseCode() == 200)
                return true;
        }
    	catch (MalformedURLException e1) { Log.w("EngNetworks", "Check online error: " + e1.getMessage()); }
    	catch (SocketTimeoutException e2) { Log.w("EngNetworks", "Check online error: " + e2.getMessage()); }
    	catch (IOException e3) { Log.w("EngNetworks", "Check online error: " + e3.getMessage()); }
    	return false;
    }
    static public String getIpDevice(boolean useIPv4) {
        try {

            List<NetworkInterface> interfaces = Collections.list(NetworkInterface.getNetworkInterfaces());
            for (NetworkInterface intf: interfaces) {

                List<InetAddress> addrs = Collections.list(intf.getInetAddresses());
                for (InetAddress addr: addrs) {

                    if (!addr.isLoopbackAddress()) {

                        String sAddr = addr.getHostAddress().toUpperCase();
                        boolean isIPv4 = InetAddressUtils.isIPv4Address(sAddr); 
                        if (useIPv4) {
                            if (isIPv4)
                                return sAddr;
                        }
                        else {
                            if (!isIPv4) {
                                int delim = sAddr.indexOf('%'); // drop ip6 port suffix
                                return (delim < 0)? sAddr:sAddr.substring(0, delim);
                            }
                        }
                    }
                }
            }
        }
        catch (Exception e) { Log.w("EngNetworks", "Failed to get IP addresses: " + e.getMessage()); }
        return null;
    }

    static private final String IP_LIST_SEPARATOR = "*";
    static public boolean SearchIpAbort = false;

    static public String getIpList(int timeOut) { // Return all IP addresses in connected network 

    	String deviceIP = getIpDevice(true);
    	if (deviceIP != null) {

        	int dotPos = deviceIP.lastIndexOf('.');
        	String ip = deviceIP.substring(0, dotPos + 1);
        	String ipList = "";

        	short i = 0;
        	while ((++i < 255) && (!SearchIpAbort)) {

        		if (deviceIP.compareTo(ip + i) == 0)
        			continue;
				try {

					InetAddress inetAddr = InetAddress.getByName(ip + i);
					if ((inetAddr != null) && (inetAddr.isReachable(timeOut))) {

						if (ipList.isEmpty()) ipList = ip + i;
						else ipList += IP_LIST_SEPARATOR + ip + i;
					}
				}
				catch (UnknownHostException e1) { }
                catch (IOException e2) { }
        	}
        	return (ipList.isEmpty())? null:ipList;
    	}
    	return null;
    }

    ////// Bluetooth LE
    
}
