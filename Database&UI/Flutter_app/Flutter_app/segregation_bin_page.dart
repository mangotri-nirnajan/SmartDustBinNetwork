import 'package:flutter/material.dart';
import 'package:http/http.dart' as http;
import 'dart:convert';

class SegregationBinPage extends StatefulWidget {
  const SegregationBinPage({Key? key}) : super(key: key);

  @override
  _SegregationBinPageState createState() => _SegregationBinPageState();
}

class _SegregationBinPageState extends State<SegregationBinPage> {
  // Define variables for the numbers representing plastic and wet items
  int plasticCount = 0;
  int wetCount = 0;

  @override
  void initState() {
    super.initState();
    _fetchBinCounts();
  }

  // Function to fetch bin counts from Firebase
  Future<void> _fetchBinCounts() async {
    const url = 'https://iotproject-f6abf-default-rtdb.asia-southeast1.firebasedatabase.app/test.json';

    try {
      final response = await http.get(Uri.parse(url));

      if (response.statusCode == 200) {
        final data = json.decode(response.body);
        setState(() {
          plasticCount = data['plastic'] ?? 0;
          wetCount = data['wet_waste'] ?? 0;
        });
        print('Bin counts fetched successfully: Plastic - $plasticCount, Wet - $wetCount');
      } else {
        print('Failed to fetch bin counts: ${response.statusCode}');
      }
    } catch (error) {
      print('Error fetching bin counts: $error');
    }
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        backgroundColor: Theme.of(context).colorScheme.primary,
        title: const Text('Segregation Bin'),
      ),
      body: Center(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: <Widget>[
            // Label for Plastic
            const Text(
              'Plastic',
              style: TextStyle(
                fontWeight: FontWeight.bold,
                fontSize: 20.0,
              ),
            ),
            const SizedBox(height: 10), // Spacer

            // Number of plastic items
            Text(
              '$plasticCount',
              style: const TextStyle(
                fontSize: 16.0,
              ),
            ),
            const SizedBox(height: 20), // Spacer

            // Label for Wet
            const Text(
              'Wet',
              style: TextStyle(
                fontWeight: FontWeight.bold,
                fontSize: 20.0,
              ),
            ),
            const SizedBox(height: 10), // Spacer

            // Number of wet items
            Text(
              '$wetCount',
              style: const TextStyle(
                fontSize: 16.0,
              ),
            ),
          ],
        ),
      ),
    );
  }
}
