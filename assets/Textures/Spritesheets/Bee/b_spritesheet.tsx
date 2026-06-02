<?xml version="1.0" encoding="UTF-8"?>
<tileset version="1.10" tiledversion="1.12.1" name="b_spritesheet" tilewidth="256" tileheight="256" tilecount="36" columns="6">
 <image source="../../../../resources/spritesheets/Bee/Bee_Enemy.png" width="1536" height="1536"/>
 <tile id="0">
  <properties>
   <property name="loop" type="bool" value="true"/>
   <property name="name" value="idle"/>
  </properties>
  <animation>
   <frame tileid="0" duration="100"/>
   <frame tileid="1" duration="100"/>
   <frame tileid="2" duration="100"/>
   <frame tileid="3" duration="100"/>
  </animation>
 </tile>
 <tile id="6">
  <properties>
   <property name="loop" type="bool" value="false"/>
   <property name="name" value="anticipation attack"/>
  </properties>
  <animation>
   <frame tileid="6" duration="100"/>
   <frame tileid="7" duration="100"/>
   <frame tileid="8" duration="100"/>
   <frame tileid="9" duration="100"/>
   <frame tileid="10" duration="100"/>
   <frame tileid="11" duration="100"/>
  </animation>
 </tile>
 <tile id="12">
  <properties>
   <property name="loop" type="bool" value="true"/>
   <property name="name" value="charge"/>
  </properties>
  <animation>
   <frame tileid="12" duration="100"/>
   <frame tileid="13" duration="100"/>
   <frame tileid="14" duration="100"/>
  </animation>
 </tile>
 <tile id="18">
  <properties>
   <property name="loop" type="bool" value="true"/>
   <property name="name" value="hit wall"/>
  </properties>
  <animation>
   <frame tileid="18" duration="100"/>
   <frame tileid="19" duration="100"/>
   <frame tileid="20" duration="100"/>
   <frame tileid="21" duration="100"/>
   <frame tileid="22" duration="100"/>
   <frame tileid="23" duration="100"/>
  </animation>
 </tile>
 <tile id="24">
  <properties>
   <property name="loop" type="bool" value="true"/>
   <property name="name" value="stunned"/>
  </properties>
  <animation>
   <frame tileid="24" duration="100"/>
   <frame tileid="25" duration="100"/>
   <frame tileid="26" duration="100"/>
   <frame tileid="27" duration="100"/>
  </animation>
 </tile>
 <tile id="30">
  <properties>
   <property name="loop" type="bool" value="false"/>
   <property name="name" value="death"/>
  </properties>
  <animation>
   <frame tileid="30" duration="100"/>
   <frame tileid="31" duration="100"/>
   <frame tileid="32" duration="100"/>
   <frame tileid="33" duration="100"/>
  </animation>
 </tile>
</tileset>
