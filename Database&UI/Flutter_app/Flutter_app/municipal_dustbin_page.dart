import 'tank_painter.dart';
import 'package:flutter/material.dart';
import 'package:url_launcher/url_launcher.dart';
import 'package:http/http.dart' as http;
import 'dart:convert';

class MunicipalDustbinPage extends StatefulWidget {
  const MunicipalDustbinPage({super.key});

  @override
  _MunicipalDustbinPageState createState() => _MunicipalDustbinPageState();
}

class _MunicipalDustbinPageState extends State<MunicipalDustbinPage> {
  double soilMoisture = 0.0;
  double quantity = 0.0;
  double destinationLat = 10.8992363;
  double destinationLong = 76.9009186;

  @override
  void initState() {
    super.initState();
    _fetchQuantity();
  }

  // Function to fetch quantity from Firebase
  // Function to fetch quantity and soil moisture from Firebase
Future<void> _fetchQuantity() async {
  const url = 'https://iotproject-f6abf-default-rtdb.asia-southeast1.firebasedatabase.app/test.json';

  try {
    final response = await http.get(Uri.parse(url));

    if (response.statusCode == 200) {
      final data = json.decode(response.body);
      setState(() {
        quantity = (data['mun_waste_level'] ?? 0) / 100; // Convert percentage to 0-1 range
        soilMoisture = (data['soil_moisture'] ?? 0) / 100; // Convert percentage to 0-1 range
      });
      print('Data fetched successfully: Quantity - $quantity, Soil Moisture - $soilMoisture');
    } else {
      print('Failed to fetch data: ${response.statusCode}');
    }
  } catch (error) {
    print('Error fetching data: $error');
  }
}

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        backgroundColor: Theme.of(context).colorScheme.primary,
        title: const Text('Municipal Dustbin'),
        actions: [
          PopupMenuButton(
            itemBuilder: (BuildContext context) {
              return [
                PopupMenuItem(
                  child: ListTile(
                    title: const Text('Change Location of the Dustbin'),
                    onTap: () {
                      _showChangeLocationDialog(context);
                    },
                  ),
                ),
              ];
            },
          ),
        ],
      ),
      body: Center(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: <Widget>[
            const Text(
              'Quantity',
              style: TextStyle(
                fontWeight: FontWeight.bold,
                fontSize: 20.0,
              ),
            ),
            SizedBox(
              width: 150.0, // Reduced width
              height: 250.0, // Increased height
              child: CustomPaint(
                painter: TankPainter(level: quantity),
              ),
            ),
            const SizedBox(height: 20.0),
            const Text(
              'Soil Moisture',
              style: TextStyle(
                fontWeight: FontWeight.bold,
                fontSize: 20.0,
              ),
            ),
            const SizedBox(height: 30.0),
            SizedBox(
              width: 140.0, // Reduced width by 30%
              child: Column(
                children: [
                  LinearProgressIndicator(
                    value: soilMoisture, // More than 50%
                    backgroundColor: Colors.grey[300],
                    color: Colors.blue,
                  ),
                  const SizedBox(height: 10.0),
                  Text(
                    '${(soilMoisture * 100).toStringAsFixed(0)}%', // Display soil moisture value as percentage
                    style: const TextStyle(
                      fontSize: 16.0,
                      fontWeight: FontWeight.bold,
                    ),
                  ),
                  const SizedBox(height: 10.0),
                  ElevatedButton(
                    onPressed: () {
                      _launchMapsUrl(destinationLat, destinationLong); // Destination latitude and longitude
                    },
                    child: const Text(
                      'Locate',
                      style: TextStyle(
                        fontSize: 16.0,
                        fontWeight: FontWeight.bold,
                      ),
                    ),
                  ),
                ],
              ),
            ),
          ],
        ),
      ),
    );
  }

  // Function to launch Google Maps with directions to the specified latitude and longitude
  void _launchMapsUrl(double lat, double long) async {
    // Construct the URL with the destination coordinates
    final url = 'https://www.google.com/maps/dir/?api=1&destination=$lat,$long';

    // Launch the URL
    if (await canLaunch(url)) {
      await launch(url);
    } else {
      throw 'Could not launch $url';
    }
  }

  // Function to show dialog for changing location of the dustbin
  Future<void> _showChangeLocationDialog(BuildContext context) async {
    double newLat = destinationLat;
    double newLong = destinationLong;

    return showDialog(
      context: context,
      builder: (BuildContext context) {
        return AlertDialog(
          title: const Text('Change Location'),
          content: SingleChildScrollView(
            child: Column(
              children: [
                TextField(
                  decoration: const InputDecoration(labelText: 'Latitude'),
                  keyboardType: TextInputType.number,
                  onChanged: (value) {
                    newLat = double.tryParse(value) ?? destinationLat;
                  },
                ),
                TextField(
                  decoration: const InputDecoration(labelText: 'Longitude'),
                  keyboardType: TextInputType.number,
                  onChanged: (value) {
                    newLong = double.tryParse(value) ?? destinationLong;
                  },
                ),
              ],
            ),
          ),
          actions: [
            ElevatedButton(
              onPressed: () {
                setState(() {
                  destinationLat = newLat;
                  destinationLong = newLong;
                });
                Navigator.of(context).pop();
              },
              child: const Text('Save'),
            ),
            ElevatedButton(
              onPressed: () {
                Navigator.of(context).pop();
              },
              child: const Text('Cancel'),
            ),
          ],
        );
      },
    );
  }
}
