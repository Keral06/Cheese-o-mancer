<?xml version="1.0" encoding="UTF-8"?>
<tileset version="1.10" tiledversion="1.12.1" name="checkpoint" tilewidth="256" tileheight="512" tilecount="12" columns="4">
 <image source="Bandera checkpoint.png" width="1024" height="1536"/>
 <tile id="0">
  <properties>
   <property name="loop" type="bool" value="false"/>
   <property name="name" value="animation"/>
  </properties>
  <animation>
   <frame tileid="0" duration="100"/>
   <frame tileid="1" duration="100"/>
   <frame tileid="2" duration="100"/>
   <frame tileid="3" duration="100"/>
   <frame tileid="4" duration="100"/>
   <frame tileid="5" duration="100"/>
   <frame tileid="6" duration="100"/>
   <frame tileid="7" duration="100"/>
   <frame tileid="8" duration="100"/>
   <frame tileid="9" duration="100"/>
   <frame tileid="10" duration="100"/>
   <frame tileid="11" duration="100"/>
  </animation>
 </tile>
 <tile id="1">
  <properties>
   <property name="loop" type="bool" value="true"/>
   <property name="name" value="static1"/>
  </properties>
  <animation>
   <frame tileid="0" duration="100"/>
  </animation>
 </tile>
 <tile id="11">
  <properties>
   <property name="loop" type="bool" value="true"/>
   <property name="name" value="static2"/>
  </properties>
  <animation>
   <frame tileid="11" duration="100"/>
  </animation>
 </tile>
</tileset>
