import 'package:demoapp3/municipal_dustbin_page.dart';
import 'package:demoapp3/private_dustbin_page.dart';
import 'package:demoapp3/segregation_bin_page.dart';
import 'package:flutter/material.dart';

class MyHomePage extends StatelessWidget {
  final String title;

  const MyHomePage({super.key, required this.title});

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        backgroundColor: Theme.of(context).colorScheme.primary,
        title: Text(title),
      ),
      body: Padding(
        padding: const EdgeInsets.symmetric(horizontal: 16.0),  // Padding on the left and right sides
        child: Column(
          children: <Widget>[
            const SizedBox(height: 16),  // Space at the top of the first button
            Expanded(
              child: SizedBox(
                width: double.infinity,
                child: ElevatedButton(
                  style: ButtonStyle(
                    backgroundColor: MaterialStateProperty.all<Color>(Color.fromARGB(255, 52, 168, 246)),
                    padding: MaterialStateProperty.all<EdgeInsets>(EdgeInsets.all(20)),
                    shape: MaterialStateProperty.all<RoundedRectangleBorder>(
                      RoundedRectangleBorder(
                        borderRadius: BorderRadius.circular(18.0),
                      ),
                    ),
                  ),
                  onPressed: () {
                    Navigator.push(
                      context,
                      MaterialPageRoute(builder: (context) => const PrivateDustbinPage()),
                    );
                  },
                  child: const Text(
                    'Private Dustbin',
                    style: TextStyle(
                      color: Colors.white,
                      fontSize: 24,  // Increase font size
                    ),
                  ),
                ),
              ),
            ),
            const SizedBox(height: 16),  // Space between buttons
            Expanded(
              child: SizedBox(
                width: double.infinity,
                child: ElevatedButton(
                  style: ButtonStyle(
                    backgroundColor: MaterialStateProperty.all<Color>(Color.fromARGB(255, 79, 216, 189)),
                    padding: MaterialStateProperty.all<EdgeInsets>(EdgeInsets.all(20)),
                    shape: MaterialStateProperty.all<RoundedRectangleBorder>(
                      RoundedRectangleBorder(
                        borderRadius: BorderRadius.circular(18.0),
                      ),
                    ),
                  ),
                  onPressed: () {
                    Navigator.push(
                      context,
                      MaterialPageRoute(builder: (context) => const MunicipalDustbinPage()),
                    );
                  },
                  child: const Text(
                    'Municipal Dustbin',
                    style: TextStyle(
                      color: Colors.white,
                      fontSize: 24,  // Increase font size
                    ),
                  ),
                ),
              ),
            ),
            const SizedBox(height: 16),  // Space between buttons
            Expanded(
              child: SizedBox(
                width: double.infinity,
                child: ElevatedButton(
                  style: ButtonStyle(
                    backgroundColor: MaterialStateProperty.all<Color>(Color.fromARGB(255, 110, 255, 146)),
                    padding: MaterialStateProperty.all<EdgeInsets>(EdgeInsets.all(20)),
                    shape: MaterialStateProperty.all<RoundedRectangleBorder>(
                      RoundedRectangleBorder(
                        borderRadius: BorderRadius.circular(18.0),
                      ),
                    ),
                  ),
                  onPressed: () {
                    Navigator.push(
                      context,
                      MaterialPageRoute(builder: (context) => SegregationBinPage()),
                    );
                  },
                  child: const Text(
                    'Segregation bin',
                    style: TextStyle(
                      color: Colors.white,
                      fontSize: 24,  // Increase font size
                    ),
                  ),
                ),
              ),
            ),
            const SizedBox(height: 16),  // Space at the bottom of the last button
          ],
        ),
      ),
    );
  }
}
