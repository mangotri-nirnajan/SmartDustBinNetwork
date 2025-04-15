import 'package:flutter/material.dart';
import 'package:http/http.dart' as http;
import 'dart:convert';

import 'tank_painter.dart';

class PrivateDustbinPage extends StatefulWidget {
  const PrivateDustbinPage({super.key});

  @override
  _PrivateDustbinPageState createState() => _PrivateDustbinPageState();
}

class _PrivateDustbinPageState extends State<PrivateDustbinPage> {
  int _isSwitched = 0;  // Changed to int
  double quant1 = 0;

  @override
  void initState() {
    super.initState();
    _fetchQuantity();
  }

  // Function to fetch quantity from Firebase
  Future<void> _fetchQuantity() async {
    const url = 'https://iotproject-f6abf-default-rtdb.asia-southeast1.firebasedatabase.app/test.json';

    try {
      final response = await http.get(Uri.parse(url));

      if (response.statusCode == 200) {
        final data = json.decode(response.body);
        setState(() {
          quant1 = (data['int'] ?? 0) / 100; // Convert percentage to 0-1 range
        });
        print('Quantity fetched successfully: $quant1');
      } else {
        print('Failed to fetch quantity: ${response.statusCode}');
      }
    } catch (error) {
      print('Error fetching quantity: $error');
    }
  }

  // Function to update switch state in Firebase
  Future<void> _updateSwitchState(int value) async {
    const url = 'https://iotproject-f6abf-default-rtdb.asia-southeast1.firebasedatabase.app/bin.json';

    try {
      final response = await http.put(
        Uri.parse(url),
        body: json.encode({'switchState': value}),
      );

      if (response.statusCode == 200) {
        print('Switch state updated successfully');
      } else {
        print('Failed to update switch state: ${response.statusCode}');
      }
    } catch (error) {
      print('Error updating switch state: $error');
    }
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        backgroundColor: Theme.of(context).colorScheme.primary,
        title: const Text('Private Dustbin'),
      ),
      body: Center(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: <Widget>[
            const Padding(
              padding: EdgeInsets.only(bottom: 16.0),
              child: Text(
                'Quantity',
                style: TextStyle(
                  fontWeight: FontWeight.bold,
                  fontSize: 20.0,  // Increased font size
                ),
              ),
            ),
            SizedBox(
              width: 150.0,  // Adjusted width
              height: 250.0,  // Adjusted height
              child: CustomPaint(
                painter: TankPainter(level: quant1),
              ),
            ),
            const SizedBox(height: 20),  // Space between buttons and container
            Row(
              mainAxisAlignment: MainAxisAlignment.center,
              children: [
                const Text(
                  'Close',
                  style: TextStyle(fontSize: 18.0),  // Increased font size
                ),
                Switch(
                  value: _isSwitched == 1,
                  onChanged: (value) {
                    setState(() {
                      _isSwitched = value ? 1 : 0;
                    });
                    _updateSwitchState(_isSwitched);
                  },
                ),
                const Text(
                  'Open',
                  style: TextStyle(fontSize: 18.0),  // Increased font size
                ),
              ],
            ),
          ],
        ),
      ),
    );
  }
}
