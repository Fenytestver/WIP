Particle - Ubidots webhook configuration
========================================

Create webhook
--------------

- Open https://console.particle.io/integrations
- Add 'New Integration'

Event name
----------
status

URL
---
https://industrial.api.ubidots.com/api/v1.6/devices/{{{PARTICLE_DEVICE_ID}}}

Request Type
------------
POST

Format
------
Json

== Device ==
Any

== Advanced Settings ==
-----------------
[ ] Default [X] Custom

Contents:
```
{
  "deviceId": "{{{deviceId}}}",
  "event": {
    "value": 0,
    "context": {
      "name": "{{{PARTICLE_EVENT_NAME}}}"
    }
  },
  "data": "{{{PARTICLE_EVENT_VALUE}}}",
  "coreid": "{{{PARTICLE_DEVICE_ID}}}",
  "published_at": "{{{PARTICLE_PUBLISHED_AT}}}",
  "technical": "{{{technical}}}",
  "critical": "{{{critical}}}",
  "alarm": {
    "value": 0,
    "context": {
      "alarm": "{{{alarm}}}"
    }
  },
  "waterLevel": "{{{waterLevel}}}",
  "leakSensor": "{{{leakSensor}}}",
  "mode": "{{{mode}}}",
  "rpm1": "{{{rpm1}}}",
  "rpm2": "{{{rpm2}}}",
  "uptime": "{{{uptime}}}",
  "battery": "{{battery}}",
  "rssi": "{{{rssi}}}",
  "bars": "{{{bars}}}"
}
```

Http headers
------------

X-Auth-Token [... insert the auth token here... ]

How to get X-Auth-Token?
------------------------

- Open https://industrial.ubidots.com/accounts/me/apikeys
- Copy "Default token" (OR for better security, create a new tocken for this Web hook)
- Paste token to as "X-Auth-Token" value
- Save Webhook

LCD Text Webhook
================

Event name: lcdtext



```
{
  "lcdtext": {
    "value": 0,
    "context": {
      "name": "{{{PARTICLE_EVENT_VALUE}}}"
    }
  }
}
```

Disply lcd html
---------------

```
<div style="background-image:url('https://i.imgur.com/AQ9AxYG.jpg');background-size: cover;background-repeat: no-repeat; padding: 60px 60px 45px 60px;">
<pre style="display:block;width:230px;line-height: 22px; overflow:hidden; font-size: 20px; font-family: monospace;word-break: break-all;white-space: pre-wrap; color: #C6FAFF; ">
{{context.name}}  
</pre>
</div>
<p style="margin: 0;">{{timestamp}}</p>
```
