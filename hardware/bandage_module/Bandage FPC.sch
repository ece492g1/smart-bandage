<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE eagle SYSTEM "eagle.dtd">
<eagle version="6.6.0">
<drawing>
<settings>
<setting alwaysvectorfont="yes"/>
<setting verticaltext="up"/>
</settings>
<grid distance="0.1" unitdist="inch" unit="inch" style="lines" multiple="1" display="no" altdistance="0.01" altunitdist="inch" altunit="inch"/>
<layers>
<layer number="1" name="Top" color="4" fill="1" visible="no" active="no"/>
<layer number="16" name="Bottom" color="1" fill="1" visible="no" active="no"/>
<layer number="17" name="Pads" color="2" fill="1" visible="no" active="no"/>
<layer number="18" name="Vias" color="2" fill="1" visible="no" active="no"/>
<layer number="19" name="Unrouted" color="6" fill="1" visible="no" active="no"/>
<layer number="20" name="Dimension" color="15" fill="1" visible="no" active="no"/>
<layer number="21" name="tPlace" color="16" fill="1" visible="no" active="no"/>
<layer number="22" name="bPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="23" name="tOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="24" name="bOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="25" name="tNames" color="7" fill="1" visible="no" active="no"/>
<layer number="26" name="bNames" color="7" fill="1" visible="no" active="no"/>
<layer number="27" name="tValues" color="7" fill="1" visible="no" active="no"/>
<layer number="28" name="bValues" color="7" fill="1" visible="no" active="no"/>
<layer number="29" name="tStop" color="7" fill="3" visible="no" active="no"/>
<layer number="30" name="bStop" color="7" fill="6" visible="no" active="no"/>
<layer number="31" name="tCream" color="7" fill="4" visible="no" active="no"/>
<layer number="32" name="bCream" color="7" fill="5" visible="no" active="no"/>
<layer number="33" name="tFinish" color="6" fill="3" visible="no" active="no"/>
<layer number="34" name="bFinish" color="6" fill="6" visible="no" active="no"/>
<layer number="35" name="tGlue" color="7" fill="4" visible="no" active="no"/>
<layer number="36" name="bGlue" color="7" fill="5" visible="no" active="no"/>
<layer number="37" name="tTest" color="7" fill="1" visible="no" active="no"/>
<layer number="38" name="bTest" color="7" fill="1" visible="no" active="no"/>
<layer number="39" name="tKeepout" color="4" fill="11" visible="no" active="no"/>
<layer number="40" name="bKeepout" color="1" fill="11" visible="no" active="no"/>
<layer number="41" name="tRestrict" color="4" fill="10" visible="no" active="no"/>
<layer number="42" name="bRestrict" color="1" fill="10" visible="no" active="no"/>
<layer number="43" name="vRestrict" color="2" fill="10" visible="no" active="no"/>
<layer number="44" name="Drills" color="7" fill="1" visible="no" active="no"/>
<layer number="45" name="Holes" color="7" fill="1" visible="no" active="no"/>
<layer number="46" name="Milling" color="3" fill="1" visible="no" active="no"/>
<layer number="47" name="Measures" color="7" fill="1" visible="no" active="no"/>
<layer number="48" name="Document" color="7" fill="1" visible="no" active="no"/>
<layer number="49" name="Reference" color="7" fill="1" visible="no" active="no"/>
<layer number="50" name="dxf" color="7" fill="1" visible="no" active="no"/>
<layer number="51" name="tDocu" color="14" fill="1" visible="no" active="no"/>
<layer number="52" name="bDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="53" name="tGND_GNDA" color="7" fill="9" visible="no" active="no"/>
<layer number="54" name="bGND_GNDA" color="1" fill="9" visible="no" active="no"/>
<layer number="56" name="wert" color="7" fill="1" visible="no" active="no"/>
<layer number="57" name="tCAD" color="7" fill="1" visible="no" active="no"/>
<layer number="59" name="tCarbon" color="7" fill="1" visible="no" active="no"/>
<layer number="60" name="bCarbon" color="7" fill="1" visible="no" active="no"/>
<layer number="90" name="Modules" color="5" fill="1" visible="yes" active="yes"/>
<layer number="91" name="Nets" color="2" fill="1" visible="yes" active="yes"/>
<layer number="92" name="Busses" color="1" fill="1" visible="yes" active="yes"/>
<layer number="93" name="Pins" color="2" fill="1" visible="no" active="yes"/>
<layer number="94" name="Symbols" color="4" fill="1" visible="yes" active="yes"/>
<layer number="95" name="Names" color="7" fill="1" visible="yes" active="yes"/>
<layer number="96" name="Values" color="7" fill="1" visible="yes" active="yes"/>
<layer number="97" name="Info" color="7" fill="1" visible="yes" active="yes"/>
<layer number="98" name="Guide" color="6" fill="1" visible="yes" active="yes"/>
<layer number="99" name="SpiceOrder" color="7" fill="1" visible="yes" active="yes"/>
<layer number="100" name="Muster" color="7" fill="1" visible="no" active="no"/>
<layer number="101" name="Patch_Top" color="12" fill="4" visible="no" active="yes"/>
<layer number="102" name="Vscore" color="7" fill="1" visible="no" active="yes"/>
<layer number="103" name="tMap" color="7" fill="1" visible="no" active="yes"/>
<layer number="104" name="Name" color="16" fill="1" visible="no" active="yes"/>
<layer number="105" name="tPlate" color="7" fill="1" visible="yes" active="yes"/>
<layer number="106" name="bPlate" color="7" fill="1" visible="no" active="yes"/>
<layer number="107" name="Crop" color="7" fill="1" visible="no" active="yes"/>
<layer number="108" name="tplace-old" color="10" fill="1" visible="no" active="yes"/>
<layer number="109" name="ref-old" color="11" fill="1" visible="no" active="yes"/>
<layer number="110" name="fp0" color="7" fill="1" visible="no" active="yes"/>
<layer number="111" name="LPC17xx" color="7" fill="1" visible="no" active="yes"/>
<layer number="112" name="tSilk" color="7" fill="1" visible="no" active="yes"/>
<layer number="113" name="IDFDebug" color="4" fill="1" visible="no" active="yes"/>
<layer number="116" name="Patch_BOT" color="9" fill="4" visible="no" active="yes"/>
<layer number="118" name="Rect_Pads" color="7" fill="1" visible="yes" active="yes"/>
<layer number="121" name="_tsilk" color="7" fill="1" visible="no" active="yes"/>
<layer number="122" name="_bsilk" color="7" fill="1" visible="no" active="yes"/>
<layer number="123" name="tTestmark" color="7" fill="1" visible="no" active="yes"/>
<layer number="124" name="bTestmark" color="7" fill="1" visible="no" active="yes"/>
<layer number="125" name="_tNames" color="7" fill="1" visible="no" active="yes"/>
<layer number="126" name="_bNames" color="7" fill="1" visible="no" active="yes"/>
<layer number="127" name="_tValues" color="7" fill="1" visible="no" active="yes"/>
<layer number="128" name="_bValues" color="7" fill="1" visible="no" active="yes"/>
<layer number="129" name="Mask" color="7" fill="1" visible="yes" active="yes"/>
<layer number="131" name="tAdjust" color="7" fill="1" visible="no" active="yes"/>
<layer number="132" name="bAdjust" color="7" fill="1" visible="no" active="yes"/>
<layer number="144" name="Drill_legend" color="7" fill="1" visible="no" active="yes"/>
<layer number="150" name="Notes" color="7" fill="1" visible="no" active="yes"/>
<layer number="151" name="HeatSink" color="7" fill="1" visible="no" active="yes"/>
<layer number="152" name="_bDocu" color="7" fill="1" visible="no" active="yes"/>
<layer number="153" name="FabDoc1" color="7" fill="1" visible="no" active="yes"/>
<layer number="154" name="FabDoc2" color="7" fill="1" visible="no" active="yes"/>
<layer number="155" name="FabDoc3" color="7" fill="1" visible="no" active="yes"/>
<layer number="199" name="Contour" color="7" fill="1" visible="no" active="yes"/>
<layer number="200" name="200bmp" color="1" fill="10" visible="no" active="yes"/>
<layer number="201" name="201bmp" color="2" fill="10" visible="no" active="yes"/>
<layer number="202" name="202bmp" color="3" fill="10" visible="no" active="yes"/>
<layer number="203" name="203bmp" color="4" fill="10" visible="no" active="yes"/>
<layer number="204" name="204bmp" color="5" fill="10" visible="no" active="yes"/>
<layer number="205" name="205bmp" color="6" fill="10" visible="no" active="yes"/>
<layer number="206" name="206bmp" color="7" fill="10" visible="no" active="yes"/>
<layer number="207" name="207bmp" color="8" fill="10" visible="no" active="yes"/>
<layer number="208" name="208bmp" color="9" fill="10" visible="no" active="yes"/>
<layer number="209" name="209bmp" color="7" fill="1" visible="no" active="yes"/>
<layer number="210" name="210bmp" color="7" fill="1" visible="no" active="yes"/>
<layer number="211" name="211bmp" color="7" fill="1" visible="no" active="yes"/>
<layer number="212" name="212bmp" color="7" fill="1" visible="no" active="yes"/>
<layer number="213" name="213bmp" color="7" fill="1" visible="no" active="yes"/>
<layer number="214" name="214bmp" color="7" fill="1" visible="no" active="yes"/>
<layer number="215" name="215bmp" color="7" fill="1" visible="no" active="yes"/>
<layer number="216" name="216bmp" color="7" fill="1" visible="no" active="yes"/>
<layer number="217" name="217bmp" color="18" fill="1" visible="no" active="no"/>
<layer number="218" name="218bmp" color="19" fill="1" visible="no" active="no"/>
<layer number="219" name="219bmp" color="20" fill="1" visible="no" active="no"/>
<layer number="220" name="220bmp" color="21" fill="1" visible="no" active="no"/>
<layer number="221" name="221bmp" color="22" fill="1" visible="no" active="no"/>
<layer number="222" name="222bmp" color="23" fill="1" visible="no" active="no"/>
<layer number="223" name="223bmp" color="24" fill="1" visible="no" active="no"/>
<layer number="224" name="224bmp" color="25" fill="1" visible="no" active="no"/>
<layer number="225" name="225bmp" color="7" fill="1" visible="yes" active="yes"/>
<layer number="226" name="226bmp" color="7" fill="1" visible="yes" active="yes"/>
<layer number="227" name="227bmp" color="7" fill="1" visible="yes" active="yes"/>
<layer number="228" name="228bmp" color="7" fill="1" visible="yes" active="yes"/>
<layer number="229" name="229bmp" color="7" fill="1" visible="yes" active="yes"/>
<layer number="230" name="230bmp" color="7" fill="1" visible="yes" active="yes"/>
<layer number="231" name="231bmp" color="7" fill="1" visible="yes" active="yes"/>
<layer number="248" name="Housing" color="7" fill="1" visible="no" active="yes"/>
<layer number="249" name="Edge" color="7" fill="1" visible="no" active="yes"/>
<layer number="250" name="Descript" color="3" fill="1" visible="no" active="no"/>
<layer number="251" name="SMDround" color="12" fill="11" visible="no" active="no"/>
<layer number="254" name="cooling" color="7" fill="1" visible="no" active="yes"/>
<layer number="255" name="routoute" color="7" fill="1" visible="yes" active="yes"/>
</layers>
<schematic xreflabel="%F%N/%S.%C%R" xrefpart="/%S.%C%R">
<libraries>
<library name="LilyPad-Wearables">
<description>&lt;h3&gt;SparkFun Electronics' preferred foot prints&lt;/h3&gt;
In this library you'll find all manner of sewable boards, textile footprints, etc.&lt;br&gt;&lt;br&gt;
We've spent an enormous amount of time creating and checking these footprints and parts, but it is the end user's responsibility to ensure correctness and suitablity for a given componet or application. If you enjoy using this library, please buy one of our products at www.sparkfun.com.
&lt;br&gt;&lt;br&gt;
&lt;b&gt;Licensing:&lt;/b&gt;Creative Commons ShareAlike 4.0 International - https://creativecommons.org/licenses/by-sa/4.0/
&lt;br&gt;&lt;br&gt;
You are welcome to use this library for commercial purposes. For attribution, we ask that when you begin to sell your device using our footprint, you email us with a link to the product being sold. We want bragging rights that we helped (in a very small part) to create your 8th world wonder. We would like the opportunity to feature your device on our homepage.</description>
<packages>
<package name="SIMPLE_HOLE_SM">
<pad name="P$1" x="0" y="0" drill="1.778" diameter="2.286"/>
</package>
</packages>
<symbols>
<symbol name="SIMPLE_HOLE_SM">
<circle x="0" y="0" radius="1.481059375" width="0.254" layer="94"/>
<pin name="P$1" x="-2.54" y="0" visible="off" length="point"/>
<wire x1="-2.54" y1="0" x2="-1.524" y2="0" width="0.254" layer="94"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="SEW_HOLE-SMALL" prefix="H">
<description>.070" hole with 20mil annular ring</description>
<gates>
<gate name="G$1" symbol="SIMPLE_HOLE_SM" x="0" y="0"/>
</gates>
<devices>
<device name="" package="SIMPLE_HOLE_SM">
<connects>
<connect gate="G$1" pin="P$1" pad="P$1"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="Michael Blouin Library">
<packages>
<package name="MSOP8">
<description>&lt;b&gt;8M, 8-Lead, 0.118" Wide, Miniature Small Outline Package&lt;/b&gt;&lt;p&gt;
MSOP&lt;br&gt;
8M-Package doc1097.pdf</description>
<wire x1="-1.48" y1="1.23" x2="-1.23" y2="1.48" width="0.1524" layer="21" curve="-90" cap="flat"/>
<wire x1="1.23" y1="1.48" x2="1.48" y2="1.23" width="0.1524" layer="21" curve="-90"/>
<wire x1="1.23" y1="-1.49" x2="1.48" y2="-1.24" width="0.1524" layer="21" curve="90"/>
<wire x1="-1.48" y1="-1.24" x2="-1.23" y2="-1.49" width="0.1524" layer="21" curve="90" cap="flat"/>
<wire x1="1.24" y1="-1.49" x2="-1.22" y2="-1.49" width="0.1524" layer="21"/>
<wire x1="-1.22" y1="1.48" x2="1.24" y2="1.48" width="0.1524" layer="21"/>
<wire x1="-1.48" y1="1.23" x2="-1.48" y2="-1.23" width="0.1524" layer="21"/>
<wire x1="1.48" y1="-1.24" x2="1.48" y2="1.23" width="0.1524" layer="21"/>
<circle x="-0.65" y="-0.65" radius="0.325" width="0.254" layer="21"/>
<smd name="1" x="-0.975" y="-2.25" dx="0.4" dy="1.3" layer="1"/>
<smd name="2" x="-0.325" y="-2.25" dx="0.4" dy="1.3" layer="1"/>
<smd name="3" x="0.325" y="-2.25" dx="0.4" dy="1.3" layer="1"/>
<smd name="4" x="0.975" y="-2.25" dx="0.4" dy="1.3" layer="1"/>
<smd name="5" x="0.975" y="2.25" dx="0.4" dy="1.3" layer="1"/>
<smd name="6" x="0.325" y="2.25" dx="0.4" dy="1.3" layer="1"/>
<smd name="7" x="-0.325" y="2.25" dx="0.4" dy="1.3" layer="1"/>
<smd name="8" x="-0.975" y="2.25" dx="0.4" dy="1.3" layer="1"/>
<text x="-2.54" y="-1.27" size="0.4064" layer="25" rot="R90">&gt;NAME</text>
<text x="2.54" y="-1.27" size="0.4064" layer="27" rot="R90">&gt;VALUE</text>
<rectangle x1="-1.175" y1="-2.45" x2="-0.775" y2="-1.55" layer="51"/>
<rectangle x1="-0.525" y1="-2.45" x2="-0.125" y2="-1.55" layer="51"/>
<rectangle x1="0.125" y1="-2.45" x2="0.525" y2="-1.55" layer="51"/>
<rectangle x1="0.775" y1="-2.45" x2="1.175" y2="-1.55" layer="51"/>
<rectangle x1="0.775" y1="1.55" x2="1.175" y2="2.45" layer="51"/>
<rectangle x1="0.125" y1="1.55" x2="0.525" y2="2.45" layer="51"/>
<rectangle x1="-0.525" y1="1.55" x2="-0.125" y2="2.45" layer="51"/>
<rectangle x1="-1.175" y1="1.55" x2="-0.775" y2="2.45" layer="51"/>
</package>
</packages>
<symbols>
<symbol name="MCP9808">
<pin name="SDA" x="-17.78" y="7.62" visible="pin" length="middle"/>
<pin name="SCL" x="-17.78" y="2.54" visible="pin" length="middle"/>
<pin name="ALERT" x="-17.78" y="-2.54" visible="pin" length="middle"/>
<pin name="GND" x="-17.78" y="-7.62" visible="pin" length="middle"/>
<pin name="A2" x="17.78" y="-7.62" visible="pin" length="middle" rot="R180"/>
<pin name="A1" x="17.78" y="-2.54" visible="pin" length="middle" rot="R180"/>
<pin name="A0" x="17.78" y="2.54" visible="pin" length="middle" rot="R180"/>
<pin name="VDD" x="17.78" y="7.62" visible="pin" length="middle" rot="R180"/>
<wire x1="-12.7" y1="-15.24" x2="-12.7" y2="15.24" width="0.254" layer="94"/>
<wire x1="-12.7" y1="15.24" x2="12.7" y2="15.24" width="0.254" layer="94"/>
<wire x1="12.7" y1="15.24" x2="12.7" y2="-15.24" width="0.254" layer="94"/>
<wire x1="12.7" y1="-15.24" x2="-12.7" y2="-15.24" width="0.254" layer="94"/>
<text x="-10.16" y="17.78" size="1.27" layer="94">&gt;NAME</text>
<text x="2.54" y="-5.08" size="1.27" layer="94" rot="R90">TPS9808</text>
</symbol>
</symbols>
<devicesets>
<deviceset name="MCP9808">
<description>MCP9808 Maximum Accuracy Digital Temperature Sensor</description>
<gates>
<gate name="G$1" symbol="MCP9808" x="0" y="0"/>
</gates>
<devices>
<device name="" package="MSOP8">
<connects>
<connect gate="G$1" pin="A0" pad="7"/>
<connect gate="G$1" pin="A1" pad="6"/>
<connect gate="G$1" pin="A2" pad="5"/>
<connect gate="G$1" pin="ALERT" pad="3"/>
<connect gate="G$1" pin="GND" pad="4"/>
<connect gate="G$1" pin="SCL" pad="2"/>
<connect gate="G$1" pin="SDA" pad="1"/>
<connect gate="G$1" pin="VDD" pad="8"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="supply1">
<description>&lt;b&gt;Supply Symbols&lt;/b&gt;&lt;p&gt;
 GND, VCC, 0V, +5V, -5V, etc.&lt;p&gt;
 Please keep in mind, that these devices are necessary for the
 automatic wiring of the supply signals.&lt;p&gt;
 The pin name defined in the symbol is identical to the net which is to be wired automatically.&lt;p&gt;
 In this library the device names are the same as the pin names of the symbols, therefore the correct signal names appear next to the supply symbols in the schematic.&lt;p&gt;
 &lt;author&gt;Created by librarian@cadsoft.de&lt;/author&gt;</description>
<packages>
</packages>
<symbols>
<symbol name="+3V3">
<wire x1="1.27" y1="-1.905" x2="0" y2="0" width="0.254" layer="94"/>
<wire x1="0" y1="0" x2="-1.27" y2="-1.905" width="0.254" layer="94"/>
<text x="-2.54" y="-5.08" size="1.778" layer="96" rot="R90">&gt;VALUE</text>
<pin name="+3V3" x="0" y="-2.54" visible="off" length="short" direction="sup" rot="R90"/>
</symbol>
<symbol name="GND">
<wire x1="-1.905" y1="0" x2="1.905" y2="0" width="0.254" layer="94"/>
<text x="-2.54" y="-2.54" size="1.778" layer="96">&gt;VALUE</text>
<pin name="GND" x="0" y="2.54" visible="off" length="short" direction="sup" rot="R270"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="+3V3" prefix="+3V3">
<description>&lt;b&gt;SUPPLY SYMBOL&lt;/b&gt;</description>
<gates>
<gate name="G$1" symbol="+3V3" x="0" y="0"/>
</gates>
<devices>
<device name="">
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="GND" prefix="GND">
<description>&lt;b&gt;SUPPLY SYMBOL&lt;/b&gt;</description>
<gates>
<gate name="1" symbol="GND" x="0" y="0"/>
</gates>
<devices>
<device name="">
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="SparkFun-Connectors">
<description>&lt;h3&gt;SparkFun Electronics' preferred foot prints&lt;/h3&gt;
In this library you'll find connectors and sockets- basically anything that can be plugged into or onto.&lt;br&gt;&lt;br&gt;
We've spent an enormous amount of time creating and checking these footprints and parts, but it is the end user's responsibility to ensure correctness and suitablity for a given componet or application. If you enjoy using this library, please buy one of our products at www.sparkfun.com.
&lt;br&gt;&lt;br&gt;
&lt;b&gt;Licensing:&lt;/b&gt; Creative Commons ShareAlike 4.0 International - https://creativecommons.org/licenses/by-sa/4.0/ 
&lt;br&gt;&lt;br&gt;
You are welcome to use this library for commercial purposes. For attribution, we ask that when you begin to sell your device using our footprint, you email us with a link to the product being sold. We want bragging rights that we helped (in a very small part) to create your 8th world wonder. We would like the opportunity to feature your device on our homepage.</description>
<packages>
<package name="AVR_ICSP">
<description>&lt;b&gt;PIN HEADER&lt;/b&gt;</description>
<wire x1="-1.905" y1="6.35" x2="-2.54" y2="5.715" width="0.1524" layer="21"/>
<wire x1="-2.54" y1="4.445" x2="-1.905" y2="3.81" width="0.1524" layer="21"/>
<wire x1="-1.905" y1="3.81" x2="-2.54" y2="3.175" width="0.1524" layer="21"/>
<wire x1="-2.54" y1="1.905" x2="-1.905" y2="1.27" width="0.1524" layer="21"/>
<wire x1="-1.905" y1="1.27" x2="-2.54" y2="0.635" width="0.1524" layer="21"/>
<wire x1="-2.54" y1="-0.635" x2="-1.905" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-1.905" y1="-1.27" x2="-2.54" y2="-1.905" width="0.1524" layer="21"/>
<wire x1="-2.54" y1="-3.175" x2="-1.905" y2="-3.81" width="0.1524" layer="21"/>
<wire x1="-1.905" y1="6.35" x2="1.905" y2="6.35" width="0.1524" layer="21"/>
<wire x1="1.905" y1="6.35" x2="2.54" y2="5.715" width="0.1524" layer="21"/>
<wire x1="2.54" y1="5.715" x2="2.54" y2="4.445" width="0.1524" layer="21"/>
<wire x1="2.54" y1="4.445" x2="1.905" y2="3.81" width="0.1524" layer="21"/>
<wire x1="1.905" y1="3.81" x2="2.54" y2="3.175" width="0.1524" layer="21"/>
<wire x1="2.54" y1="3.175" x2="2.54" y2="1.905" width="0.1524" layer="21"/>
<wire x1="2.54" y1="1.905" x2="1.905" y2="1.27" width="0.1524" layer="21"/>
<wire x1="1.905" y1="1.27" x2="2.54" y2="0.635" width="0.1524" layer="21"/>
<wire x1="2.54" y1="0.635" x2="2.54" y2="-0.635" width="0.1524" layer="21"/>
<wire x1="2.54" y1="-0.635" x2="1.905" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="1.905" y1="-1.27" x2="2.54" y2="-1.905" width="0.1524" layer="21"/>
<wire x1="2.54" y1="-1.905" x2="2.54" y2="-3.175" width="0.1524" layer="21"/>
<wire x1="2.54" y1="-3.175" x2="1.905" y2="-3.81" width="0.1524" layer="21"/>
<wire x1="1.905" y1="3.81" x2="-1.905" y2="3.81" width="0.1524" layer="21"/>
<wire x1="1.905" y1="1.27" x2="-1.905" y2="1.27" width="0.1524" layer="21"/>
<wire x1="1.905" y1="-1.27" x2="-1.905" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="1.905" y1="-3.81" x2="-1.905" y2="-3.81" width="0.1524" layer="21"/>
<wire x1="-2.54" y1="-1.905" x2="-2.54" y2="-3.175" width="0.1524" layer="21"/>
<wire x1="-2.54" y1="0.635" x2="-2.54" y2="-0.635" width="0.1524" layer="21"/>
<wire x1="-2.54" y1="3.175" x2="-2.54" y2="1.905" width="0.1524" layer="21"/>
<wire x1="-2.54" y1="5.715" x2="-2.54" y2="4.445" width="0.1524" layer="21"/>
<wire x1="-1.905" y1="-3.81" x2="-2.54" y2="-4.445" width="0.1524" layer="21"/>
<wire x1="-2.54" y1="-5.715" x2="-1.905" y2="-6.35" width="0.1524" layer="21"/>
<wire x1="1.905" y1="-3.81" x2="2.54" y2="-4.445" width="0.1524" layer="21"/>
<wire x1="2.54" y1="-4.445" x2="2.54" y2="-5.715" width="0.1524" layer="21"/>
<wire x1="2.54" y1="-5.715" x2="1.905" y2="-6.35" width="0.1524" layer="21"/>
<wire x1="1.905" y1="-6.35" x2="-1.905" y2="-6.35" width="0.1524" layer="21"/>
<wire x1="-2.54" y1="-4.445" x2="-2.54" y2="-5.715" width="0.1524" layer="21"/>
<wire x1="-3.175" y1="5.715" x2="-3.175" y2="4.445" width="0.2032" layer="21"/>
<pad name="1" x="-1.27" y="5.08" drill="1.016" diameter="1.8796" shape="octagon" rot="R270"/>
<pad name="2" x="1.27" y="5.08" drill="1.016" diameter="1.8796" shape="octagon" rot="R270"/>
<pad name="3" x="-1.27" y="2.54" drill="1.016" diameter="1.8796" shape="octagon" rot="R270"/>
<pad name="4" x="1.27" y="2.54" drill="1.016" diameter="1.8796" shape="octagon" rot="R270"/>
<pad name="5" x="-1.27" y="0" drill="1.016" diameter="1.8796" shape="octagon" rot="R270"/>
<pad name="6" x="1.27" y="0" drill="1.016" diameter="1.8796" shape="octagon" rot="R270"/>
<pad name="7" x="-1.27" y="-2.54" drill="1.016" diameter="1.8796" shape="octagon" rot="R270"/>
<pad name="8" x="1.27" y="-2.54" drill="1.016" diameter="1.8796" shape="octagon" rot="R270"/>
<pad name="9" x="-1.27" y="-5.08" drill="1.016" diameter="1.8796" shape="octagon" rot="R270"/>
<pad name="10" x="1.27" y="-5.08" drill="1.016" diameter="1.8796" shape="octagon" rot="R270"/>
<text x="-2.794" y="-3.302" size="0.4064" layer="25" rot="R90">&gt;NAME</text>
<text x="-2.794" y="-0.254" size="0.4064" layer="27" font="vector" rot="R90">&gt;VALUE</text>
<rectangle x1="-1.524" y1="4.826" x2="-1.016" y2="5.334" layer="51"/>
<rectangle x1="1.016" y1="4.826" x2="1.524" y2="5.334" layer="51"/>
<rectangle x1="1.016" y1="2.286" x2="1.524" y2="2.794" layer="51"/>
<rectangle x1="-1.524" y1="2.286" x2="-1.016" y2="2.794" layer="51"/>
<rectangle x1="1.016" y1="-0.254" x2="1.524" y2="0.254" layer="51"/>
<rectangle x1="-1.524" y1="-0.254" x2="-1.016" y2="0.254" layer="21"/>
<rectangle x1="1.016" y1="-5.334" x2="1.524" y2="-4.826" layer="51"/>
<rectangle x1="-1.524" y1="-5.334" x2="-1.016" y2="-4.826" layer="51"/>
<rectangle x1="-1.524" y1="-2.794" x2="-1.016" y2="-2.286" layer="51"/>
<rectangle x1="1.016" y1="-2.794" x2="1.524" y2="-2.286" layer="51"/>
</package>
<package name="2X5-RA">
<wire x1="-3.175" y1="5.715" x2="-3.175" y2="4.445" width="0.2032" layer="21"/>
<wire x1="2.8" y1="6.3" x2="5.3" y2="6.3" width="0.2032" layer="21"/>
<wire x1="5.3" y1="6.3" x2="5.3" y2="-6.3" width="0.2032" layer="21"/>
<wire x1="5.3" y1="-6.3" x2="2.8" y2="-6.3" width="0.2032" layer="21"/>
<wire x1="2.8" y1="-6.3" x2="2.8" y2="6.3" width="0.2032" layer="21"/>
<wire x1="5.3" y1="0" x2="11.3" y2="0" width="0.127" layer="51"/>
<wire x1="5.3" y1="-2.54" x2="11.3" y2="-2.54" width="0.127" layer="51"/>
<wire x1="5.3" y1="-5.08" x2="11.3" y2="-5.08" width="0.127" layer="51"/>
<wire x1="5.3" y1="2.54" x2="11.3" y2="2.54" width="0.127" layer="51"/>
<wire x1="5.3" y1="5.08" x2="11.3" y2="5.08" width="0.127" layer="51"/>
<wire x1="8.2" y1="7" x2="8.2" y2="-6.9" width="0.127" layer="51"/>
<wire x1="13.8" y1="6.3" x2="13.8" y2="-6.3" width="0.127" layer="51"/>
<wire x1="5.3" y1="6.3" x2="13.8" y2="6.3" width="0.127" layer="51"/>
<wire x1="5.3" y1="-6.3" x2="13.8" y2="-6.3" width="0.127" layer="51"/>
<pad name="1" x="-1.27" y="5.08" drill="1.016" diameter="1.8796" shape="octagon" rot="R270"/>
<pad name="2" x="1.27" y="5.08" drill="1.016" diameter="1.8796" shape="octagon" rot="R270"/>
<pad name="3" x="-1.27" y="2.54" drill="1.016" diameter="1.8796" shape="octagon" rot="R270"/>
<pad name="4" x="1.27" y="2.54" drill="1.016" diameter="1.8796" shape="octagon" rot="R270"/>
<pad name="5" x="-1.27" y="0" drill="1.016" diameter="1.8796" shape="octagon" rot="R270"/>
<pad name="6" x="1.27" y="0" drill="1.016" diameter="1.8796" shape="octagon" rot="R270"/>
<pad name="7" x="-1.27" y="-2.54" drill="1.016" diameter="1.8796" shape="octagon" rot="R270"/>
<pad name="8" x="1.27" y="-2.54" drill="1.016" diameter="1.8796" shape="octagon" rot="R270"/>
<pad name="9" x="-1.27" y="-5.08" drill="1.016" diameter="1.8796" shape="octagon" rot="R270"/>
<pad name="10" x="1.27" y="-5.08" drill="1.016" diameter="1.8796" shape="octagon" rot="R270"/>
<text x="-2" y="-8.2" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="-1.9" y="7" size="1.27" layer="27" font="vector">&gt;VALUE</text>
<rectangle x1="-1.524" y1="4.826" x2="-1.016" y2="5.334" layer="51" rot="R270"/>
<rectangle x1="1.016" y1="4.826" x2="1.524" y2="5.334" layer="51" rot="R270"/>
<rectangle x1="1.016" y1="2.286" x2="1.524" y2="2.794" layer="51" rot="R270"/>
<rectangle x1="-1.524" y1="2.286" x2="-1.016" y2="2.794" layer="51" rot="R270"/>
<rectangle x1="1.016" y1="-0.254" x2="1.524" y2="0.254" layer="51" rot="R270"/>
<rectangle x1="-1.524" y1="-0.254" x2="-1.016" y2="0.254" layer="51" rot="R270"/>
<rectangle x1="1.016" y1="-2.794" x2="1.524" y2="-2.286" layer="51" rot="R270"/>
<rectangle x1="-1.524" y1="-2.794" x2="-1.016" y2="-2.286" layer="51" rot="R270"/>
<rectangle x1="1.016" y1="-5.334" x2="1.524" y2="-4.826" layer="51" rot="R270"/>
<rectangle x1="-1.524" y1="-5.334" x2="-1.016" y2="-4.826" layer="51" rot="R270"/>
<rectangle x1="-1.524" y1="-2.794" x2="-1.016" y2="-2.286" layer="51" rot="R270"/>
<rectangle x1="1.016" y1="-2.794" x2="1.524" y2="-2.286" layer="51" rot="R270"/>
</package>
<package name="2X5-RAF">
<wire x1="-3.175" y1="5.715" x2="-3.175" y2="4.445" width="0.2032" layer="21"/>
<wire x1="2.7" y1="6.3" x2="11.2" y2="6.3" width="0.2032" layer="21"/>
<wire x1="11.2" y1="6.3" x2="11.2" y2="-6.3" width="0.2032" layer="21"/>
<wire x1="11.2" y1="-6.3" x2="2.7" y2="-6.3" width="0.2032" layer="21"/>
<wire x1="2.7" y1="-6.3" x2="2.7" y2="6.3" width="0.2032" layer="21"/>
<wire x1="8.2" y1="7" x2="8.2" y2="-6.9" width="0.127" layer="51"/>
<pad name="1" x="-1.27" y="5.08" drill="1.016" diameter="1.8796" shape="octagon" rot="R270"/>
<pad name="2" x="1.27" y="5.08" drill="1.016" diameter="1.8796" shape="octagon" rot="R270"/>
<pad name="3" x="-1.27" y="2.54" drill="1.016" diameter="1.8796" shape="octagon" rot="R270"/>
<pad name="4" x="1.27" y="2.54" drill="1.016" diameter="1.8796" shape="octagon" rot="R270"/>
<pad name="5" x="-1.27" y="0" drill="1.016" diameter="1.8796" shape="octagon" rot="R270"/>
<pad name="6" x="1.27" y="0" drill="1.016" diameter="1.8796" shape="octagon" rot="R270"/>
<pad name="7" x="-1.27" y="-2.54" drill="1.016" diameter="1.8796" shape="octagon" rot="R270"/>
<pad name="8" x="1.27" y="-2.54" drill="1.016" diameter="1.8796" shape="octagon" rot="R270"/>
<pad name="9" x="-1.27" y="-5.08" drill="1.016" diameter="1.8796" shape="octagon" rot="R270"/>
<pad name="10" x="1.27" y="-5.08" drill="1.016" diameter="1.8796" shape="octagon" rot="R270"/>
<text x="-2" y="-8.2" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="-1.9" y="7" size="1.27" layer="27" font="vector">&gt;VALUE</text>
<rectangle x1="-1.524" y1="4.826" x2="-1.016" y2="5.334" layer="51" rot="R270"/>
<rectangle x1="1.016" y1="4.826" x2="1.524" y2="5.334" layer="51" rot="R270"/>
<rectangle x1="1.016" y1="2.286" x2="1.524" y2="2.794" layer="51" rot="R270"/>
<rectangle x1="-1.524" y1="2.286" x2="-1.016" y2="2.794" layer="51" rot="R270"/>
<rectangle x1="1.016" y1="-0.254" x2="1.524" y2="0.254" layer="51" rot="R270"/>
<rectangle x1="-1.524" y1="-0.254" x2="-1.016" y2="0.254" layer="51" rot="R270"/>
<rectangle x1="1.016" y1="-2.794" x2="1.524" y2="-2.286" layer="51" rot="R270"/>
<rectangle x1="-1.524" y1="-2.794" x2="-1.016" y2="-2.286" layer="51" rot="R270"/>
<rectangle x1="1.016" y1="-5.334" x2="1.524" y2="-4.826" layer="51" rot="R270"/>
<rectangle x1="-1.524" y1="-5.334" x2="-1.016" y2="-4.826" layer="51" rot="R270"/>
<rectangle x1="-1.524" y1="-2.794" x2="-1.016" y2="-2.286" layer="51" rot="R270"/>
<rectangle x1="1.016" y1="-2.794" x2="1.524" y2="-2.286" layer="51" rot="R270"/>
</package>
<package name="2X5-SHROUDED">
<wire x1="-2.775" y1="5.715" x2="-2.775" y2="4.445" width="0.2032" layer="21"/>
<wire x1="4.5" y1="10.1" x2="4.5" y2="-10.1" width="0.2032" layer="21"/>
<wire x1="-4.5" y1="-10.1" x2="-4.5" y2="-2.2" width="0.2032" layer="21"/>
<wire x1="-4.5" y1="-2.2" x2="-4.5" y2="2.2" width="0.2032" layer="21"/>
<wire x1="-4.5" y1="2.2" x2="-4.5" y2="10.1" width="0.2032" layer="21"/>
<wire x1="-4.5" y1="10.1" x2="4.4" y2="10.1" width="0.2032" layer="21"/>
<wire x1="4.5" y1="-10.1" x2="-4.5" y2="-10.1" width="0.2032" layer="21"/>
<wire x1="-3.4" y1="9" x2="3.4" y2="9" width="0.2032" layer="51"/>
<wire x1="3.4" y1="9" x2="3.4" y2="-9" width="0.2032" layer="51"/>
<wire x1="-3.4" y1="-9" x2="3.4" y2="-9" width="0.2032" layer="51"/>
<wire x1="-4.5" y1="2.2" x2="-3" y2="2.2" width="0.2032" layer="21"/>
<wire x1="-3" y1="2.2" x2="-3" y2="-2.2" width="0.2032" layer="21"/>
<wire x1="-3" y1="-2.2" x2="-4.5" y2="-2.2" width="0.2032" layer="21"/>
<wire x1="-3.4" y1="9" x2="-3.4" y2="2.2" width="0.2032" layer="51"/>
<wire x1="-3.4" y1="-9" x2="-3.4" y2="-2.2" width="0.2032" layer="51"/>
<pad name="1" x="-1.27" y="5.08" drill="1.016" diameter="1.8796" shape="octagon" rot="R270"/>
<pad name="2" x="1.27" y="5.08" drill="1.016" diameter="1.8796" shape="octagon" rot="R270"/>
<pad name="3" x="-1.27" y="2.54" drill="1.016" diameter="1.8796" shape="octagon" rot="R270"/>
<pad name="4" x="1.27" y="2.54" drill="1.016" diameter="1.8796" shape="octagon" rot="R270"/>
<pad name="5" x="-1.27" y="0" drill="1.016" diameter="1.8796" shape="octagon" rot="R270"/>
<pad name="6" x="1.27" y="0" drill="1.016" diameter="1.8796" shape="octagon" rot="R270"/>
<pad name="7" x="-1.27" y="-2.54" drill="1.016" diameter="1.8796" shape="octagon" rot="R270"/>
<pad name="8" x="1.27" y="-2.54" drill="1.016" diameter="1.8796" shape="octagon" rot="R270"/>
<pad name="9" x="-1.27" y="-5.08" drill="1.016" diameter="1.8796" shape="octagon" rot="R270"/>
<pad name="10" x="1.27" y="-5.08" drill="1.016" diameter="1.8796" shape="octagon" rot="R270"/>
<text x="-2.921" y="10.414" size="0.4064" layer="27" font="vector">&gt;VALUE</text>
<text x="-2.921" y="-10.922" size="0.4064" layer="25">&gt;NAME</text>
<rectangle x1="-1.524" y1="4.826" x2="-1.016" y2="5.334" layer="51"/>
<rectangle x1="1.016" y1="4.826" x2="1.524" y2="5.334" layer="51"/>
<rectangle x1="1.016" y1="2.286" x2="1.524" y2="2.794" layer="51"/>
<rectangle x1="-1.524" y1="2.286" x2="-1.016" y2="2.794" layer="51"/>
<rectangle x1="1.016" y1="-0.254" x2="1.524" y2="0.254" layer="51"/>
<rectangle x1="-1.524" y1="-0.254" x2="-1.016" y2="0.254" layer="51"/>
<rectangle x1="1.016" y1="-2.794" x2="1.524" y2="-2.286" layer="51"/>
<rectangle x1="-1.524" y1="-2.794" x2="-1.016" y2="-2.286" layer="51"/>
<rectangle x1="1.016" y1="-5.334" x2="1.524" y2="-4.826" layer="51"/>
<rectangle x1="-1.524" y1="-5.334" x2="-1.016" y2="-4.826" layer="51"/>
<rectangle x1="-1.524" y1="-2.794" x2="-1.016" y2="-2.286" layer="51"/>
<rectangle x1="1.016" y1="-2.794" x2="1.524" y2="-2.286" layer="51"/>
</package>
<package name="2X5-SHROUDED_LOCK">
<wire x1="-2.775" y1="5.715" x2="-2.775" y2="4.445" width="0.2032" layer="21"/>
<wire x1="4.5" y1="10.1" x2="4.5" y2="-10.1" width="0.2032" layer="21"/>
<wire x1="-4.5" y1="-10.1" x2="-4.5" y2="-2.2" width="0.2032" layer="21"/>
<wire x1="-4.5" y1="-2.2" x2="-4.5" y2="2.2" width="0.2032" layer="21"/>
<wire x1="-4.5" y1="2.2" x2="-4.5" y2="10.1" width="0.2032" layer="21"/>
<wire x1="-4.5" y1="10.1" x2="4.4" y2="10.1" width="0.2032" layer="21"/>
<wire x1="4.5" y1="-10.1" x2="-4.5" y2="-10.1" width="0.2032" layer="21"/>
<wire x1="-3.4" y1="9" x2="3.4" y2="9" width="0.2032" layer="51"/>
<wire x1="3.4" y1="9" x2="3.4" y2="-9" width="0.2032" layer="51"/>
<wire x1="-3.4" y1="-9" x2="3.4" y2="-9" width="0.2032" layer="51"/>
<wire x1="-4.5" y1="2.2" x2="-3" y2="2.2" width="0.2032" layer="21"/>
<wire x1="-3" y1="2.2" x2="-3" y2="-2.2" width="0.2032" layer="21"/>
<wire x1="-3" y1="-2.2" x2="-4.5" y2="-2.2" width="0.2032" layer="21"/>
<wire x1="-3.4" y1="9" x2="-3.4" y2="2.2" width="0.2032" layer="51"/>
<wire x1="-3.4" y1="-9" x2="-3.4" y2="-2.2" width="0.2032" layer="51"/>
<pad name="1" x="-1.397" y="5.08" drill="1.016" diameter="1.8796" shape="octagon" rot="R270"/>
<pad name="2" x="1.397" y="5.08" drill="1.016" diameter="1.8796" shape="octagon" rot="R270"/>
<pad name="3" x="-1.397" y="2.54" drill="1.016" diameter="1.8796" shape="octagon" rot="R270"/>
<pad name="4" x="1.397" y="2.54" drill="1.016" diameter="1.8796" shape="octagon" rot="R270"/>
<pad name="5" x="-1.397" y="0" drill="1.016" diameter="1.8796" shape="octagon" rot="R270"/>
<pad name="6" x="1.397" y="0" drill="1.016" diameter="1.8796" shape="octagon" rot="R270"/>
<pad name="7" x="-1.397" y="-2.54" drill="1.016" diameter="1.8796" shape="octagon" rot="R270"/>
<pad name="8" x="1.397" y="-2.54" drill="1.016" diameter="1.8796" shape="octagon" rot="R270"/>
<pad name="9" x="-1.397" y="-5.08" drill="1.016" diameter="1.8796" shape="octagon" rot="R270"/>
<pad name="10" x="1.397" y="-5.08" drill="1.016" diameter="1.8796" shape="octagon" rot="R270"/>
<text x="-2.921" y="10.414" size="0.4064" layer="27" font="vector">&gt;VALUE</text>
<text x="-2.921" y="-10.922" size="0.4064" layer="104">&gt;NAME</text>
<rectangle x1="-1.524" y1="4.826" x2="-1.016" y2="5.334" layer="51" rot="R270"/>
<rectangle x1="1.016" y1="4.826" x2="1.524" y2="5.334" layer="51" rot="R270"/>
<rectangle x1="1.016" y1="2.286" x2="1.524" y2="2.794" layer="51" rot="R270"/>
<rectangle x1="-1.524" y1="2.286" x2="-1.016" y2="2.794" layer="51" rot="R270"/>
<rectangle x1="1.016" y1="-0.254" x2="1.524" y2="0.254" layer="51" rot="R270"/>
<rectangle x1="-1.524" y1="-0.254" x2="-1.016" y2="0.254" layer="51" rot="R270"/>
<rectangle x1="1.016" y1="-2.794" x2="1.524" y2="-2.286" layer="51" rot="R270"/>
<rectangle x1="-1.524" y1="-2.794" x2="-1.016" y2="-2.286" layer="51" rot="R270"/>
<rectangle x1="1.016" y1="-5.334" x2="1.524" y2="-4.826" layer="51" rot="R270"/>
<rectangle x1="-1.524" y1="-5.334" x2="-1.016" y2="-4.826" layer="51" rot="R270"/>
<rectangle x1="-1.524" y1="-2.794" x2="-1.016" y2="-2.286" layer="51" rot="R270"/>
<rectangle x1="1.016" y1="-2.794" x2="1.524" y2="-2.286" layer="51" rot="R270"/>
</package>
<package name="2X5-SHROUDED_SMD">
<wire x1="-2.775" y1="5.715" x2="-2.775" y2="4.445" width="0.2032" layer="21"/>
<wire x1="4.5" y1="10.1" x2="4.5" y2="-10.1" width="0.2032" layer="21"/>
<wire x1="-4.5" y1="-10.1" x2="-4.5" y2="-2.2" width="0.2032" layer="21"/>
<wire x1="-4.5" y1="-2.2" x2="-4.5" y2="2.2" width="0.2032" layer="21"/>
<wire x1="-4.5" y1="2.2" x2="-4.5" y2="10.1" width="0.2032" layer="21"/>
<wire x1="-4.5" y1="10.1" x2="4.4" y2="10.1" width="0.2032" layer="21"/>
<wire x1="4.5" y1="-10.1" x2="-4.5" y2="-10.1" width="0.2032" layer="21"/>
<wire x1="-3.4" y1="9" x2="3.4" y2="9" width="0.2032" layer="51"/>
<wire x1="3.4" y1="9" x2="3.4" y2="-9" width="0.2032" layer="51"/>
<wire x1="-3.4" y1="-9" x2="3.4" y2="-9" width="0.2032" layer="51"/>
<wire x1="-4.5" y1="2.2" x2="-3" y2="2.2" width="0.2032" layer="21"/>
<wire x1="-3" y1="2.2" x2="-3" y2="-2.2" width="0.2032" layer="21"/>
<wire x1="-3" y1="-2.2" x2="-4.5" y2="-2.2" width="0.2032" layer="21"/>
<wire x1="-3.4" y1="9" x2="-3.4" y2="2.2" width="0.2032" layer="51"/>
<wire x1="-3.4" y1="-9" x2="-3.4" y2="-2.2" width="0.2032" layer="51"/>
<smd name="1" x="-2.794" y="5.08" dx="4.15" dy="1" layer="1" roundness="50"/>
<smd name="2" x="2.794" y="5.08" dx="4.15" dy="1" layer="1" roundness="50"/>
<smd name="3" x="-2.794" y="2.54" dx="4.15" dy="1" layer="1" roundness="50"/>
<smd name="4" x="2.794" y="2.54" dx="4.15" dy="1" layer="1" roundness="50"/>
<smd name="5" x="-2.794" y="0" dx="4.15" dy="1" layer="1" roundness="50"/>
<smd name="6" x="2.794" y="0" dx="4.15" dy="1" layer="1" roundness="50"/>
<smd name="7" x="-2.794" y="-2.54" dx="4.15" dy="1" layer="1" roundness="50"/>
<smd name="8" x="2.794" y="-2.54" dx="4.15" dy="1" layer="1" roundness="50"/>
<smd name="9" x="-2.794" y="-5.08" dx="4.15" dy="1" layer="1" roundness="50"/>
<smd name="10" x="2.794" y="-5.08" dx="4.15" dy="1" layer="1" roundness="50"/>
<text x="-2.921" y="10.414" size="0.4064" layer="27" font="vector">&gt;VALUE</text>
<text x="-2.921" y="-10.922" size="0.4064" layer="104">&gt;NAME</text>
<rectangle x1="-1.524" y1="4.826" x2="-1.016" y2="5.334" layer="51"/>
<rectangle x1="1.016" y1="4.826" x2="1.524" y2="5.334" layer="51"/>
<rectangle x1="1.016" y1="2.286" x2="1.524" y2="2.794" layer="51"/>
<rectangle x1="-1.524" y1="2.286" x2="-1.016" y2="2.794" layer="51"/>
<rectangle x1="1.016" y1="-0.254" x2="1.524" y2="0.254" layer="51"/>
<rectangle x1="-1.524" y1="-0.254" x2="-1.016" y2="0.254" layer="51"/>
<rectangle x1="1.016" y1="-2.794" x2="1.524" y2="-2.286" layer="51"/>
<rectangle x1="-1.524" y1="-2.794" x2="-1.016" y2="-2.286" layer="51"/>
<rectangle x1="1.016" y1="-5.334" x2="1.524" y2="-4.826" layer="51"/>
<rectangle x1="-1.524" y1="-5.334" x2="-1.016" y2="-4.826" layer="51"/>
<rectangle x1="-1.524" y1="-2.794" x2="-1.016" y2="-2.286" layer="51"/>
<rectangle x1="1.016" y1="-2.794" x2="1.524" y2="-2.286" layer="51"/>
</package>
<package name="2X5_NOSILK">
<pad name="1" x="-1.27" y="5.08" drill="1.016" diameter="1.8796" shape="octagon" rot="R270"/>
<pad name="2" x="1.27" y="5.08" drill="1.016" diameter="1.8796" shape="octagon" rot="R270"/>
<pad name="3" x="-1.27" y="2.54" drill="1.016" diameter="1.8796" shape="octagon" rot="R270"/>
<pad name="4" x="1.27" y="2.54" drill="1.016" diameter="1.8796" shape="octagon" rot="R270"/>
<pad name="5" x="-1.27" y="0" drill="1.016" diameter="1.8796" shape="octagon" rot="R270"/>
<pad name="6" x="1.27" y="0" drill="1.016" diameter="1.8796" shape="octagon" rot="R270"/>
<pad name="7" x="-1.27" y="-2.54" drill="1.016" diameter="1.8796" shape="octagon" rot="R270"/>
<pad name="8" x="1.27" y="-2.54" drill="1.016" diameter="1.8796" shape="octagon" rot="R270"/>
<pad name="9" x="-1.27" y="-5.08" drill="1.016" diameter="1.8796" shape="octagon" rot="R270"/>
<pad name="10" x="1.27" y="-5.08" drill="1.016" diameter="1.8796" shape="octagon" rot="R270"/>
<text x="-2.794" y="-3.302" size="0.4064" layer="25" rot="R90">&gt;NAME</text>
<text x="-2.794" y="-0.254" size="0.4064" layer="27" font="vector" rot="R90">&gt;VALUE</text>
<rectangle x1="-1.524" y1="4.826" x2="-1.016" y2="5.334" layer="51"/>
<rectangle x1="1.016" y1="4.826" x2="1.524" y2="5.334" layer="51"/>
<rectangle x1="1.016" y1="2.286" x2="1.524" y2="2.794" layer="51"/>
<rectangle x1="-1.524" y1="2.286" x2="-1.016" y2="2.794" layer="51"/>
<rectangle x1="-1.524" y1="-0.254" x2="-1.016" y2="0.254" layer="51"/>
<rectangle x1="1.016" y1="-5.334" x2="1.524" y2="-4.826" layer="51"/>
<rectangle x1="-1.524" y1="-5.334" x2="-1.016" y2="-4.826" layer="51"/>
<rectangle x1="-1.524" y1="-2.794" x2="-1.016" y2="-2.286" layer="51"/>
<rectangle x1="1.016" y1="-2.794" x2="1.524" y2="-2.286" layer="51"/>
<rectangle x1="1.016" y1="-0.254" x2="1.524" y2="0.254" layer="51"/>
</package>
<package name="2X5_PTH_SILK_.05">
<pad name="1" x="0" y="6.35" drill="0.4318" shape="square"/>
<pad name="2" x="1.27" y="6.35" drill="0.4318"/>
<pad name="3" x="0" y="5.08" drill="0.4318"/>
<pad name="4" x="1.27" y="5.08" drill="0.4318"/>
<pad name="5" x="0" y="3.81" drill="0.4318"/>
<pad name="6" x="1.27" y="3.81" drill="0.4318"/>
<pad name="7" x="0" y="2.54" drill="0.4318"/>
<pad name="8" x="1.27" y="2.54" drill="0.4318"/>
<pad name="9" x="0" y="1.27" drill="0.4318"/>
<pad name="10" x="1.27" y="1.27" drill="0.4318"/>
<wire x1="0.635" y1="6.985" x2="0.762" y2="7.112" width="0.127" layer="21"/>
<wire x1="0.762" y1="7.112" x2="1.778" y2="7.112" width="0.127" layer="21"/>
<wire x1="1.778" y1="7.112" x2="2.032" y2="6.858" width="0.127" layer="21"/>
<wire x1="2.032" y1="6.858" x2="2.032" y2="5.842" width="0.127" layer="21"/>
<wire x1="2.032" y1="5.842" x2="1.905" y2="5.715" width="0.127" layer="21"/>
<wire x1="1.905" y1="5.715" x2="2.032" y2="5.588" width="0.127" layer="21"/>
<wire x1="2.032" y1="5.588" x2="2.032" y2="4.572" width="0.127" layer="21"/>
<wire x1="2.032" y1="4.572" x2="1.905" y2="4.445" width="0.127" layer="21"/>
<wire x1="1.905" y1="4.445" x2="2.032" y2="4.318" width="0.127" layer="21"/>
<wire x1="2.032" y1="4.318" x2="2.032" y2="3.302" width="0.127" layer="21"/>
<wire x1="2.032" y1="3.302" x2="1.905" y2="3.175" width="0.127" layer="21"/>
<wire x1="1.905" y1="3.175" x2="2.032" y2="3.048" width="0.127" layer="21"/>
<wire x1="2.032" y1="3.048" x2="2.032" y2="2.032" width="0.127" layer="21"/>
<wire x1="2.032" y1="2.032" x2="1.905" y2="1.905" width="0.127" layer="21"/>
<wire x1="1.905" y1="1.905" x2="2.032" y2="1.778" width="0.127" layer="21"/>
<wire x1="2.032" y1="1.778" x2="2.032" y2="0.762" width="0.127" layer="21"/>
<wire x1="2.032" y1="0.762" x2="1.778" y2="0.508" width="0.127" layer="21"/>
<wire x1="1.778" y1="0.508" x2="0.762" y2="0.508" width="0.127" layer="21"/>
<wire x1="0.762" y1="0.508" x2="0.635" y2="0.635" width="0.127" layer="21"/>
<wire x1="0.635" y1="0.635" x2="0.508" y2="0.508" width="0.127" layer="21"/>
<wire x1="0.508" y1="0.508" x2="-0.508" y2="0.508" width="0.127" layer="21"/>
<wire x1="-0.508" y1="0.508" x2="-0.762" y2="0.762" width="0.127" layer="21"/>
<wire x1="-0.762" y1="0.762" x2="-0.762" y2="1.778" width="0.127" layer="21"/>
<wire x1="-0.762" y1="1.778" x2="-0.635" y2="1.905" width="0.127" layer="21"/>
<wire x1="-0.635" y1="1.905" x2="-0.762" y2="2.032" width="0.127" layer="21"/>
<wire x1="-0.762" y1="2.032" x2="-0.762" y2="3.048" width="0.127" layer="21"/>
<wire x1="-0.762" y1="3.048" x2="-0.635" y2="3.175" width="0.127" layer="21"/>
<wire x1="-0.635" y1="3.175" x2="-0.762" y2="3.302" width="0.127" layer="21"/>
<wire x1="-0.762" y1="3.302" x2="-0.762" y2="4.318" width="0.127" layer="21"/>
<wire x1="-0.762" y1="4.318" x2="-0.635" y2="4.445" width="0.127" layer="21"/>
<wire x1="-0.635" y1="4.445" x2="-0.762" y2="4.572" width="0.127" layer="21"/>
<wire x1="-0.762" y1="4.572" x2="-0.762" y2="5.588" width="0.127" layer="21"/>
<wire x1="-0.762" y1="5.588" x2="-0.635" y2="5.715" width="0.127" layer="21"/>
<wire x1="-0.635" y1="5.715" x2="-0.762" y2="5.842" width="0.127" layer="21"/>
<wire x1="-0.762" y1="5.842" x2="-0.762" y2="6.858" width="0.127" layer="21"/>
<wire x1="-0.762" y1="6.858" x2="-0.508" y2="7.112" width="0.127" layer="21"/>
<wire x1="-0.508" y1="7.112" x2="0.508" y2="7.112" width="0.127" layer="21"/>
<wire x1="0.508" y1="7.112" x2="0.635" y2="6.985" width="0.127" layer="21"/>
<wire x1="-0.508" y1="7.366" x2="0.508" y2="7.366" width="0.127" layer="21"/>
</package>
<package name="2X5-SHROUDED-NS">
<wire x1="-2.775" y1="5.715" x2="-2.775" y2="4.445" width="0.2032" layer="21"/>
<wire x1="4.5" y1="10.1" x2="4.5" y2="-10.1" width="0.2032" layer="51"/>
<wire x1="-4.5" y1="-10.1" x2="-4.5" y2="-2.2" width="0.2032" layer="51"/>
<wire x1="-4.5" y1="-2.2" x2="-4.5" y2="2.2" width="0.2032" layer="51"/>
<wire x1="-4.5" y1="2.2" x2="-4.5" y2="10.1" width="0.2032" layer="51"/>
<wire x1="-4.5" y1="10.1" x2="4.4" y2="10.1" width="0.2032" layer="51"/>
<wire x1="4.5" y1="-10.1" x2="-4.5" y2="-10.1" width="0.2032" layer="51"/>
<wire x1="-3.4" y1="9" x2="3.4" y2="9" width="0.2032" layer="51"/>
<wire x1="3.4" y1="9" x2="3.4" y2="-9" width="0.2032" layer="51"/>
<wire x1="-3.4" y1="-9" x2="3.4" y2="-9" width="0.2032" layer="51"/>
<wire x1="-4.5" y1="2.2" x2="-3" y2="2.2" width="0.2032" layer="51"/>
<wire x1="-3" y1="2.2" x2="-3" y2="-2.2" width="0.2032" layer="51"/>
<wire x1="-3" y1="-2.2" x2="-4.5" y2="-2.2" width="0.2032" layer="51"/>
<wire x1="-3.4" y1="9" x2="-3.4" y2="2.2" width="0.2032" layer="51"/>
<wire x1="-3.4" y1="-9" x2="-3.4" y2="-2.2" width="0.2032" layer="51"/>
<pad name="1" x="-1.27" y="5.08" drill="1.016" diameter="1.8796" shape="octagon" rot="R270"/>
<pad name="2" x="1.27" y="5.08" drill="1.016" diameter="1.8796" shape="octagon" rot="R270"/>
<pad name="3" x="-1.27" y="2.54" drill="1.016" diameter="1.8796" shape="octagon" rot="R270"/>
<pad name="4" x="1.27" y="2.54" drill="1.016" diameter="1.8796" shape="octagon" rot="R270"/>
<pad name="5" x="-1.27" y="0" drill="1.016" diameter="1.8796" shape="octagon" rot="R270"/>
<pad name="6" x="1.27" y="0" drill="1.016" diameter="1.8796" shape="octagon" rot="R270"/>
<pad name="7" x="-1.27" y="-2.54" drill="1.016" diameter="1.8796" shape="octagon" rot="R270"/>
<pad name="8" x="1.27" y="-2.54" drill="1.016" diameter="1.8796" shape="octagon" rot="R270"/>
<pad name="9" x="-1.27" y="-5.08" drill="1.016" diameter="1.8796" shape="octagon" rot="R270"/>
<pad name="10" x="1.27" y="-5.08" drill="1.016" diameter="1.8796" shape="octagon" rot="R270"/>
<text x="-2.921" y="10.414" size="0.4064" layer="27" font="vector">&gt;VALUE</text>
<text x="-2.921" y="-10.922" size="0.4064" layer="25">&gt;NAME</text>
<rectangle x1="-1.524" y1="4.826" x2="-1.016" y2="5.334" layer="51"/>
<rectangle x1="1.016" y1="4.826" x2="1.524" y2="5.334" layer="51"/>
<rectangle x1="1.016" y1="2.286" x2="1.524" y2="2.794" layer="51"/>
<rectangle x1="-1.524" y1="2.286" x2="-1.016" y2="2.794" layer="51"/>
<rectangle x1="1.016" y1="-0.254" x2="1.524" y2="0.254" layer="51"/>
<rectangle x1="-1.524" y1="-0.254" x2="-1.016" y2="0.254" layer="51"/>
<rectangle x1="1.016" y1="-2.794" x2="1.524" y2="-2.286" layer="51"/>
<rectangle x1="-1.524" y1="-2.794" x2="-1.016" y2="-2.286" layer="51"/>
<rectangle x1="1.016" y1="-5.334" x2="1.524" y2="-4.826" layer="51"/>
<rectangle x1="-1.524" y1="-5.334" x2="-1.016" y2="-4.826" layer="51"/>
<rectangle x1="-1.524" y1="-2.794" x2="-1.016" y2="-2.286" layer="51"/>
<rectangle x1="1.016" y1="-2.794" x2="1.524" y2="-2.286" layer="51"/>
</package>
</packages>
<symbols>
<symbol name="M05X2">
<wire x1="3.81" y1="-7.62" x2="-3.81" y2="-7.62" width="0.4064" layer="94"/>
<wire x1="1.27" y1="0" x2="2.54" y2="0" width="0.6096" layer="94"/>
<wire x1="1.27" y1="-2.54" x2="2.54" y2="-2.54" width="0.6096" layer="94"/>
<wire x1="1.27" y1="-5.08" x2="2.54" y2="-5.08" width="0.6096" layer="94"/>
<wire x1="-3.81" y1="7.62" x2="-3.81" y2="-7.62" width="0.4064" layer="94"/>
<wire x1="3.81" y1="-7.62" x2="3.81" y2="7.62" width="0.4064" layer="94"/>
<wire x1="-3.81" y1="7.62" x2="3.81" y2="7.62" width="0.4064" layer="94"/>
<wire x1="1.27" y1="5.08" x2="2.54" y2="5.08" width="0.6096" layer="94"/>
<wire x1="1.27" y1="2.54" x2="2.54" y2="2.54" width="0.6096" layer="94"/>
<wire x1="-1.27" y1="0" x2="-2.54" y2="0" width="0.6096" layer="94"/>
<wire x1="-1.27" y1="-2.54" x2="-2.54" y2="-2.54" width="0.6096" layer="94"/>
<wire x1="-1.27" y1="-5.08" x2="-2.54" y2="-5.08" width="0.6096" layer="94"/>
<wire x1="-1.27" y1="5.08" x2="-2.54" y2="5.08" width="0.6096" layer="94"/>
<wire x1="-1.27" y1="2.54" x2="-2.54" y2="2.54" width="0.6096" layer="94"/>
<text x="-2.54" y="-10.16" size="1.778" layer="96">&gt;VALUE</text>
<text x="-2.54" y="8.382" size="1.778" layer="95">&gt;NAME</text>
<pin name="10" x="7.62" y="-5.08" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="8" x="7.62" y="-2.54" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="6" x="7.62" y="0" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="4" x="7.62" y="2.54" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="2" x="7.62" y="5.08" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="9" x="-7.62" y="-5.08" visible="pad" length="middle" direction="pas" swaplevel="1"/>
<pin name="7" x="-7.62" y="-2.54" visible="pad" length="middle" direction="pas" swaplevel="1"/>
<pin name="5" x="-7.62" y="0" visible="pad" length="middle" direction="pas" swaplevel="1"/>
<pin name="3" x="-7.62" y="2.54" visible="pad" length="middle" direction="pas" swaplevel="1"/>
<pin name="1" x="-7.62" y="5.08" visible="pad" length="middle" direction="pas" swaplevel="1"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="M05X2" prefix="JP" uservalue="yes">
<description>&lt;b&gt;Header 5x2&lt;/b&gt;
Standard 10-pin dual row 0.1" header. Commonly used with AVR-ISP. Use with Spark Fun Electronics SKU: PRT-00778</description>
<gates>
<gate name="G$1" symbol="M05X2" x="0" y="0"/>
</gates>
<devices>
<device name="PTH" package="AVR_ICSP">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="10" pad="10"/>
<connect gate="G$1" pin="2" pad="2"/>
<connect gate="G$1" pin="3" pad="3"/>
<connect gate="G$1" pin="4" pad="4"/>
<connect gate="G$1" pin="5" pad="5"/>
<connect gate="G$1" pin="6" pad="6"/>
<connect gate="G$1" pin="7" pad="7"/>
<connect gate="G$1" pin="8" pad="8"/>
<connect gate="G$1" pin="9" pad="9"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="RA" package="2X5-RA">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="10" pad="10"/>
<connect gate="G$1" pin="2" pad="2"/>
<connect gate="G$1" pin="3" pad="3"/>
<connect gate="G$1" pin="4" pad="4"/>
<connect gate="G$1" pin="5" pad="5"/>
<connect gate="G$1" pin="6" pad="6"/>
<connect gate="G$1" pin="7" pad="7"/>
<connect gate="G$1" pin="8" pad="8"/>
<connect gate="G$1" pin="9" pad="9"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="RAF" package="2X5-RAF">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="10" pad="10"/>
<connect gate="G$1" pin="2" pad="2"/>
<connect gate="G$1" pin="3" pad="3"/>
<connect gate="G$1" pin="4" pad="4"/>
<connect gate="G$1" pin="5" pad="5"/>
<connect gate="G$1" pin="6" pad="6"/>
<connect gate="G$1" pin="7" pad="7"/>
<connect gate="G$1" pin="8" pad="8"/>
<connect gate="G$1" pin="9" pad="9"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="SHD" package="2X5-SHROUDED">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="10" pad="10"/>
<connect gate="G$1" pin="2" pad="2"/>
<connect gate="G$1" pin="3" pad="3"/>
<connect gate="G$1" pin="4" pad="4"/>
<connect gate="G$1" pin="5" pad="5"/>
<connect gate="G$1" pin="6" pad="6"/>
<connect gate="G$1" pin="7" pad="7"/>
<connect gate="G$1" pin="8" pad="8"/>
<connect gate="G$1" pin="9" pad="9"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="SHD_LOCK" package="2X5-SHROUDED_LOCK">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="10" pad="10"/>
<connect gate="G$1" pin="2" pad="2"/>
<connect gate="G$1" pin="3" pad="3"/>
<connect gate="G$1" pin="4" pad="4"/>
<connect gate="G$1" pin="5" pad="5"/>
<connect gate="G$1" pin="6" pad="6"/>
<connect gate="G$1" pin="7" pad="7"/>
<connect gate="G$1" pin="8" pad="8"/>
<connect gate="G$1" pin="9" pad="9"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="SHD_SMD" package="2X5-SHROUDED_SMD">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="10" pad="10"/>
<connect gate="G$1" pin="2" pad="2"/>
<connect gate="G$1" pin="3" pad="3"/>
<connect gate="G$1" pin="4" pad="4"/>
<connect gate="G$1" pin="5" pad="5"/>
<connect gate="G$1" pin="6" pad="6"/>
<connect gate="G$1" pin="7" pad="7"/>
<connect gate="G$1" pin="8" pad="8"/>
<connect gate="G$1" pin="9" pad="9"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="NO_SILK" package="2X5_NOSILK">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="10" pad="10"/>
<connect gate="G$1" pin="2" pad="2"/>
<connect gate="G$1" pin="3" pad="3"/>
<connect gate="G$1" pin="4" pad="4"/>
<connect gate="G$1" pin="5" pad="5"/>
<connect gate="G$1" pin="6" pad="6"/>
<connect gate="G$1" pin="7" pad="7"/>
<connect gate="G$1" pin="8" pad="8"/>
<connect gate="G$1" pin="9" pad="9"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="0.05_IN_PTH_SILK" package="2X5_PTH_SILK_.05">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="10" pad="10"/>
<connect gate="G$1" pin="2" pad="2"/>
<connect gate="G$1" pin="3" pad="3"/>
<connect gate="G$1" pin="4" pad="4"/>
<connect gate="G$1" pin="5" pad="5"/>
<connect gate="G$1" pin="6" pad="6"/>
<connect gate="G$1" pin="7" pad="7"/>
<connect gate="G$1" pin="8" pad="8"/>
<connect gate="G$1" pin="9" pad="9"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="SHD-NS" package="2X5-SHROUDED-NS">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="10" pad="10"/>
<connect gate="G$1" pin="2" pad="2"/>
<connect gate="G$1" pin="3" pad="3"/>
<connect gate="G$1" pin="4" pad="4"/>
<connect gate="G$1" pin="5" pad="5"/>
<connect gate="G$1" pin="6" pad="6"/>
<connect gate="G$1" pin="7" pad="7"/>
<connect gate="G$1" pin="8" pad="8"/>
<connect gate="G$1" pin="9" pad="9"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="Capstone Library">
<packages>
<package name="SON-6">
<smd name="6" x="-1" y="1.35" dx="0.5" dy="0.45" layer="1" rot="R90"/>
<smd name="5" x="0" y="1.35" dx="0.5" dy="0.45" layer="1" rot="R90"/>
<smd name="4" x="1" y="1.35" dx="0.5" dy="0.45" layer="1" rot="R90"/>
<smd name="3" x="1" y="-1.35" dx="0.5" dy="0.45" layer="1" rot="R90"/>
<smd name="2" x="0" y="-1.35" dx="0.5" dy="0.45" layer="1" rot="R90"/>
<smd name="1" x="-1" y="-1.35" dx="0.5" dy="0.45" layer="1" rot="R90"/>
<smd name="PAD" x="0" y="0" dx="1.5" dy="2.4" layer="1" rot="R90"/>
<wire x1="-1.5" y1="1.5" x2="-1.5" y2="-1.5" width="0.127" layer="21"/>
<wire x1="-1.5" y1="-1.5" x2="1.5" y2="-1.5" width="0.127" layer="21"/>
<wire x1="1.5" y1="-1.5" x2="1.5" y2="1.5" width="0.127" layer="21"/>
<wire x1="1.5" y1="1.5" x2="-1.5" y2="1.5" width="0.127" layer="21"/>
<rectangle x1="-1.5" y1="-1.5" x2="-0.5" y2="-0.5" layer="21"/>
<text x="-1.75" y="-1.5" size="0.6096" layer="21" rot="R90">&gt;NAME</text>
</package>
</packages>
<symbols>
<symbol name="HDC1050">
<pin name="VDD" x="-15.24" y="5.08" length="middle"/>
<pin name="GND" x="-15.24" y="0" length="middle"/>
<pin name="SDA" x="15.24" y="0" length="middle" rot="R180"/>
<pin name="SCL" x="15.24" y="5.08" length="middle" rot="R180"/>
<wire x1="-10.16" y1="10.16" x2="-10.16" y2="-5.08" width="0.254" layer="94"/>
<wire x1="-10.16" y1="-5.08" x2="10.16" y2="-5.08" width="0.254" layer="94"/>
<wire x1="10.16" y1="-5.08" x2="10.16" y2="10.16" width="0.254" layer="94"/>
<wire x1="10.16" y1="10.16" x2="-10.16" y2="10.16" width="0.254" layer="94"/>
<text x="-10.16" y="12.7" size="2.54" layer="94">&gt;NAME</text>
</symbol>
</symbols>
<devicesets>
<deviceset name="HDC1050">
<gates>
<gate name="G$1" symbol="HDC1050" x="0" y="0"/>
</gates>
<devices>
<device name="" package="SON-6">
<connects>
<connect gate="G$1" pin="GND" pad="2"/>
<connect gate="G$1" pin="SCL" pad="6"/>
<connect gate="G$1" pin="SDA" pad="1"/>
<connect gate="G$1" pin="VDD" pad="5"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
</libraries>
<attributes>
</attributes>
<variantdefs>
</variantdefs>
<classes>
<class number="0" name="default" width="0" drill="0">
</class>
</classes>
<parts>
<part name="H1" library="LilyPad-Wearables" deviceset="SEW_HOLE-SMALL" device=""/>
<part name="H2" library="LilyPad-Wearables" deviceset="SEW_HOLE-SMALL" device=""/>
<part name="H3" library="LilyPad-Wearables" deviceset="SEW_HOLE-SMALL" device=""/>
<part name="H4" library="LilyPad-Wearables" deviceset="SEW_HOLE-SMALL" device=""/>
<part name="H5" library="LilyPad-Wearables" deviceset="SEW_HOLE-SMALL" device=""/>
<part name="H6" library="LilyPad-Wearables" deviceset="SEW_HOLE-SMALL" device=""/>
<part name="H7" library="LilyPad-Wearables" deviceset="SEW_HOLE-SMALL" device=""/>
<part name="H8" library="LilyPad-Wearables" deviceset="SEW_HOLE-SMALL" device=""/>
<part name="H9" library="LilyPad-Wearables" deviceset="SEW_HOLE-SMALL" device=""/>
<part name="U$1" library="Michael Blouin Library" deviceset="MCP9808" device=""/>
<part name="U$2" library="Michael Blouin Library" deviceset="MCP9808" device=""/>
<part name="U$3" library="Michael Blouin Library" deviceset="MCP9808" device=""/>
<part name="+3V1" library="supply1" deviceset="+3V3" device=""/>
<part name="+3V2" library="supply1" deviceset="+3V3" device=""/>
<part name="+3V3" library="supply1" deviceset="+3V3" device=""/>
<part name="GND2" library="supply1" deviceset="GND" device=""/>
<part name="GND3" library="supply1" deviceset="GND" device=""/>
<part name="GND4" library="supply1" deviceset="GND" device=""/>
<part name="GND5" library="supply1" deviceset="GND" device=""/>
<part name="GND6" library="supply1" deviceset="GND" device=""/>
<part name="GND7" library="supply1" deviceset="GND" device=""/>
<part name="JP1" library="SparkFun-Connectors" deviceset="M05X2" device="NO_SILK"/>
<part name="+3V4" library="supply1" deviceset="+3V3" device=""/>
<part name="GND1" library="supply1" deviceset="GND" device=""/>
<part name="HUM" library="Capstone Library" deviceset="HDC1050" device=""/>
<part name="GND8" library="supply1" deviceset="GND" device=""/>
<part name="+3V5" library="supply1" deviceset="+3V3" device=""/>
</parts>
<sheets>
<sheet>
<plain>
<text x="12.7" y="101.6" size="1.778" layer="105">NC</text>
<text x="12.7" y="60.96" size="1.778" layer="105">NC</text>
<text x="12.7" y="20.32" size="1.778" layer="105">NC</text>
<wire x1="2.54" y1="2.54" x2="2.54" y2="132.08" width="0.1524" layer="97"/>
<wire x1="2.54" y1="132.08" x2="63.5" y2="132.08" width="0.1524" layer="97"/>
<wire x1="63.5" y1="132.08" x2="63.5" y2="2.54" width="0.1524" layer="97"/>
<wire x1="63.5" y1="2.54" x2="2.54" y2="2.54" width="0.1524" layer="97"/>
<text x="5.08" y="127" size="1.778" layer="97">Temperature Sensors</text>
</plain>
<instances>
<instance part="H1" gate="G$1" x="101.6" y="55.88" rot="R90"/>
<instance part="H2" gate="G$1" x="96.52" y="53.34" rot="R90"/>
<instance part="H3" gate="G$1" x="147.32" y="50.8" rot="R90"/>
<instance part="H4" gate="G$1" x="154.94" y="48.26" rot="R90"/>
<instance part="H5" gate="G$1" x="106.68" y="40.64" rot="R270"/>
<instance part="H6" gate="G$1" x="91.44" y="50.8" rot="R90"/>
<instance part="H7" gate="G$1" x="86.36" y="50.8" rot="R90"/>
<instance part="H8" gate="G$1" x="81.28" y="50.8" rot="R90"/>
<instance part="H9" gate="G$1" x="76.2" y="50.8" rot="R90"/>
<instance part="U$1" gate="G$1" x="35.56" y="104.14"/>
<instance part="U$2" gate="G$1" x="35.56" y="63.5"/>
<instance part="U$3" gate="G$1" x="35.56" y="22.86"/>
<instance part="+3V1" gate="G$1" x="58.42" y="116.84"/>
<instance part="+3V2" gate="G$1" x="58.42" y="76.2"/>
<instance part="+3V3" gate="G$1" x="58.42" y="35.56"/>
<instance part="GND2" gate="1" x="15.24" y="91.44"/>
<instance part="GND3" gate="1" x="15.24" y="50.8"/>
<instance part="GND4" gate="1" x="15.24" y="10.16"/>
<instance part="GND5" gate="1" x="58.42" y="91.44"/>
<instance part="GND6" gate="1" x="58.42" y="50.8"/>
<instance part="GND7" gate="1" x="58.42" y="10.16"/>
<instance part="JP1" gate="G$1" x="116.84" y="48.26"/>
<instance part="+3V4" gate="G$1" x="142.24" y="40.64" rot="R270"/>
<instance part="GND1" gate="1" x="99.06" y="40.64"/>
<instance part="HUM" gate="G$1" x="116.84" y="83.82"/>
<instance part="GND8" gate="1" x="99.06" y="78.74"/>
<instance part="+3V5" gate="G$1" x="99.06" y="93.98"/>
</instances>
<busses>
</busses>
<nets>
<net name="GND" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="GND"/>
<wire x1="17.78" y1="96.52" x2="15.24" y2="96.52" width="0.1524" layer="91"/>
<wire x1="15.24" y1="96.52" x2="15.24" y2="93.98" width="0.1524" layer="91"/>
<pinref part="GND2" gate="1" pin="GND"/>
</segment>
<segment>
<pinref part="U$2" gate="G$1" pin="GND"/>
<wire x1="17.78" y1="55.88" x2="15.24" y2="55.88" width="0.1524" layer="91"/>
<wire x1="15.24" y1="55.88" x2="15.24" y2="53.34" width="0.1524" layer="91"/>
<pinref part="GND3" gate="1" pin="GND"/>
</segment>
<segment>
<pinref part="U$3" gate="G$1" pin="GND"/>
<wire x1="17.78" y1="15.24" x2="15.24" y2="15.24" width="0.1524" layer="91"/>
<wire x1="15.24" y1="15.24" x2="15.24" y2="12.7" width="0.1524" layer="91"/>
<pinref part="GND4" gate="1" pin="GND"/>
</segment>
<segment>
<wire x1="58.42" y1="93.98" x2="58.42" y2="96.52" width="0.1524" layer="91"/>
<pinref part="U$1" gate="G$1" pin="A0"/>
<wire x1="58.42" y1="96.52" x2="58.42" y2="101.6" width="0.1524" layer="91"/>
<wire x1="58.42" y1="101.6" x2="58.42" y2="106.68" width="0.1524" layer="91"/>
<wire x1="58.42" y1="106.68" x2="53.34" y2="106.68" width="0.1524" layer="91"/>
<pinref part="U$1" gate="G$1" pin="A1"/>
<wire x1="53.34" y1="101.6" x2="58.42" y2="101.6" width="0.1524" layer="91"/>
<pinref part="U$1" gate="G$1" pin="A2"/>
<wire x1="53.34" y1="96.52" x2="58.42" y2="96.52" width="0.1524" layer="91"/>
<pinref part="GND5" gate="1" pin="GND"/>
<junction x="58.42" y="101.6"/>
<junction x="58.42" y="96.52"/>
</segment>
<segment>
<pinref part="U$2" gate="G$1" pin="A1"/>
<pinref part="GND6" gate="1" pin="GND"/>
<wire x1="53.34" y1="60.96" x2="58.42" y2="60.96" width="0.1524" layer="91"/>
<wire x1="58.42" y1="60.96" x2="58.42" y2="55.88" width="0.1524" layer="91"/>
<pinref part="U$2" gate="G$1" pin="A2"/>
<wire x1="58.42" y1="55.88" x2="58.42" y2="53.34" width="0.1524" layer="91"/>
<wire x1="53.34" y1="55.88" x2="58.42" y2="55.88" width="0.1524" layer="91"/>
<junction x="58.42" y="55.88"/>
</segment>
<segment>
<pinref part="U$3" gate="G$1" pin="A2"/>
<pinref part="GND7" gate="1" pin="GND"/>
<wire x1="53.34" y1="15.24" x2="58.42" y2="15.24" width="0.1524" layer="91"/>
<wire x1="58.42" y1="15.24" x2="58.42" y2="12.7" width="0.1524" layer="91"/>
</segment>
<segment>
<pinref part="JP1" gate="G$1" pin="7"/>
<wire x1="109.22" y1="45.72" x2="99.06" y2="45.72" width="0.1524" layer="91"/>
<wire x1="99.06" y1="45.72" x2="99.06" y2="43.18" width="0.1524" layer="91"/>
<pinref part="GND1" gate="1" pin="GND"/>
</segment>
<segment>
<pinref part="HUM" gate="G$1" pin="GND"/>
<wire x1="101.6" y1="83.82" x2="99.06" y2="83.82" width="0.1524" layer="91"/>
<wire x1="99.06" y1="83.82" x2="99.06" y2="81.28" width="0.1524" layer="91"/>
<pinref part="GND8" gate="1" pin="GND"/>
</segment>
</net>
<net name="+3V3" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="VDD"/>
<pinref part="+3V1" gate="G$1" pin="+3V3"/>
<wire x1="53.34" y1="111.76" x2="58.42" y2="111.76" width="0.1524" layer="91"/>
<wire x1="58.42" y1="111.76" x2="58.42" y2="114.3" width="0.1524" layer="91"/>
</segment>
<segment>
<pinref part="U$2" gate="G$1" pin="VDD"/>
<pinref part="+3V2" gate="G$1" pin="+3V3"/>
<wire x1="53.34" y1="71.12" x2="58.42" y2="71.12" width="0.1524" layer="91"/>
<wire x1="58.42" y1="71.12" x2="58.42" y2="73.66" width="0.1524" layer="91"/>
<pinref part="U$2" gate="G$1" pin="A0"/>
<wire x1="53.34" y1="66.04" x2="58.42" y2="66.04" width="0.1524" layer="91"/>
<wire x1="58.42" y1="66.04" x2="58.42" y2="71.12" width="0.1524" layer="91"/>
<junction x="58.42" y="71.12"/>
</segment>
<segment>
<pinref part="U$3" gate="G$1" pin="VDD"/>
<pinref part="+3V3" gate="G$1" pin="+3V3"/>
<wire x1="53.34" y1="30.48" x2="58.42" y2="30.48" width="0.1524" layer="91"/>
<wire x1="58.42" y1="30.48" x2="58.42" y2="33.02" width="0.1524" layer="91"/>
<pinref part="U$3" gate="G$1" pin="A0"/>
<wire x1="53.34" y1="25.4" x2="58.42" y2="25.4" width="0.1524" layer="91"/>
<wire x1="58.42" y1="25.4" x2="58.42" y2="30.48" width="0.1524" layer="91"/>
<pinref part="U$3" gate="G$1" pin="A1"/>
<wire x1="53.34" y1="20.32" x2="58.42" y2="20.32" width="0.1524" layer="91"/>
<wire x1="58.42" y1="20.32" x2="58.42" y2="25.4" width="0.1524" layer="91"/>
<junction x="58.42" y="25.4"/>
<junction x="58.42" y="30.48"/>
</segment>
<segment>
<pinref part="JP1" gate="G$1" pin="10"/>
<pinref part="+3V4" gate="G$1" pin="+3V3"/>
<wire x1="124.46" y1="43.18" x2="134.62" y2="43.18" width="0.1524" layer="91"/>
<wire x1="134.62" y1="43.18" x2="139.7" y2="40.64" width="0.1524" layer="91"/>
</segment>
<segment>
<pinref part="HUM" gate="G$1" pin="VDD"/>
<wire x1="101.6" y1="88.9" x2="99.06" y2="88.9" width="0.1524" layer="91"/>
<wire x1="99.06" y1="88.9" x2="99.06" y2="91.44" width="0.1524" layer="91"/>
<pinref part="+3V5" gate="G$1" pin="+3V3"/>
</segment>
</net>
<net name="SCL" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="SCL"/>
<wire x1="17.78" y1="106.68" x2="5.08" y2="106.68" width="0.1524" layer="91"/>
<label x="5.08" y="106.68" size="1.778" layer="95"/>
</segment>
<segment>
<pinref part="U$2" gate="G$1" pin="SCL"/>
<wire x1="17.78" y1="66.04" x2="5.08" y2="66.04" width="0.1524" layer="91"/>
<label x="5.08" y="66.04" size="1.778" layer="95"/>
</segment>
<segment>
<pinref part="U$3" gate="G$1" pin="SCL"/>
<wire x1="17.78" y1="25.4" x2="5.08" y2="25.4" width="0.1524" layer="91"/>
<label x="5.08" y="25.4" size="1.778" layer="95"/>
</segment>
<segment>
<pinref part="JP1" gate="G$1" pin="2"/>
<wire x1="124.46" y1="53.34" x2="142.24" y2="53.34" width="0.1524" layer="91"/>
<label x="137.16" y="53.34" size="1.778" layer="95"/>
</segment>
<segment>
<pinref part="HUM" gate="G$1" pin="SCL"/>
<wire x1="132.08" y1="88.9" x2="144.78" y2="88.9" width="0.1524" layer="91"/>
<label x="139.7" y="88.9" size="1.778" layer="95"/>
</segment>
</net>
<net name="SDA" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="SDA"/>
<wire x1="17.78" y1="111.76" x2="5.08" y2="111.76" width="0.1524" layer="91"/>
<label x="5.08" y="111.76" size="1.778" layer="95"/>
</segment>
<segment>
<pinref part="U$2" gate="G$1" pin="SDA"/>
<wire x1="17.78" y1="71.12" x2="5.08" y2="71.12" width="0.1524" layer="91"/>
<label x="5.08" y="71.12" size="1.778" layer="95"/>
</segment>
<segment>
<pinref part="U$3" gate="G$1" pin="SDA"/>
<wire x1="17.78" y1="30.48" x2="5.08" y2="30.48" width="0.1524" layer="91"/>
<label x="5.08" y="30.48" size="1.778" layer="95"/>
</segment>
<segment>
<pinref part="JP1" gate="G$1" pin="4"/>
<wire x1="124.46" y1="50.8" x2="142.24" y2="50.8" width="0.1524" layer="91"/>
<label x="137.16" y="50.8" size="1.778" layer="95"/>
</segment>
<segment>
<pinref part="HUM" gate="G$1" pin="SDA"/>
<wire x1="132.08" y1="83.82" x2="144.78" y2="83.82" width="0.1524" layer="91"/>
<label x="139.7" y="83.82" size="1.778" layer="95"/>
</segment>
</net>
<net name="N$5" class="0">
<segment>
<pinref part="JP1" gate="G$1" pin="9"/>
<wire x1="109.22" y1="43.18" x2="106.68" y2="43.18" width="0.1524" layer="91"/>
<pinref part="H5" gate="G$1" pin="P$1"/>
</segment>
</net>
<net name="N$3" class="0">
<segment>
<pinref part="JP1" gate="G$1" pin="8"/>
<pinref part="H4" gate="G$1" pin="P$1"/>
<wire x1="124.46" y1="45.72" x2="154.94" y2="45.72" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$4" class="0">
<segment>
<pinref part="JP1" gate="G$1" pin="6"/>
<wire x1="124.46" y1="48.26" x2="147.32" y2="48.26" width="0.1524" layer="91"/>
<pinref part="H3" gate="G$1" pin="P$1"/>
</segment>
</net>
<net name="AGND" class="0">
<segment>
<pinref part="H6" gate="G$1" pin="P$1"/>
<wire x1="91.44" y1="48.26" x2="86.36" y2="48.26" width="0.1524" layer="91"/>
<pinref part="H7" gate="G$1" pin="P$1"/>
<wire x1="86.36" y1="48.26" x2="81.28" y2="48.26" width="0.1524" layer="91"/>
<junction x="86.36" y="48.26"/>
<pinref part="H8" gate="G$1" pin="P$1"/>
<wire x1="81.28" y1="48.26" x2="76.2" y2="48.26" width="0.1524" layer="91"/>
<junction x="81.28" y="48.26"/>
<pinref part="H9" gate="G$1" pin="P$1"/>
<junction x="91.44" y="48.26"/>
<pinref part="JP1" gate="G$1" pin="5"/>
<wire x1="109.22" y1="48.26" x2="91.44" y2="48.26" width="0.1524" layer="91"/>
<label x="101.6" y="48.26" size="1.778" layer="95"/>
</segment>
</net>
<net name="N$1" class="0">
<segment>
<pinref part="JP1" gate="G$1" pin="3"/>
<pinref part="H2" gate="G$1" pin="P$1"/>
<wire x1="109.22" y1="50.8" x2="96.52" y2="50.8" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$2" class="0">
<segment>
<pinref part="JP1" gate="G$1" pin="1"/>
<pinref part="H1" gate="G$1" pin="P$1"/>
<wire x1="109.22" y1="53.34" x2="101.6" y2="53.34" width="0.1524" layer="91"/>
</segment>
</net>
</nets>
</sheet>
</sheets>
</schematic>
</drawing>
</eagle>
