package com.blueserial;

/**
 * Created by Manash on 2/1/2016.
 */

import android.app.PendingIntent;
import android.content.Intent;
import android.telephony.SmsManager;

public class ThiefAlert  {

    private String phoneNumber;
    private String message;
    private SmsManager smsManager;

    //Ctor
    public ThiefAlert(String phoneNumber, String message){
        this.phoneNumber = phoneNumber;
        this.message = message;
        smsManager = SmsManager.getDefault();
    }

    public void sendSMS(){
        smsManager.sendTextMessage(phoneNumber, null, message, null, null);
    }

    public void setPhoneNumber(String phoneNumber){
        this.phoneNumber = phoneNumber;
    }

    public void setMessage(String message){
        this.message = message;
    }




}
