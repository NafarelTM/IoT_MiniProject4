package com.example.iotminiproject4

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.widget.Button
import android.widget.EditText
import android.widget.TextView
import android.widget.ToggleButton

import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

class MainActivity : AppCompatActivity() {

    private lateinit var tvSuhu: TextView
    private lateinit var txtPembatas: EditText
    private lateinit var btnPembatas: Button
    private lateinit var tbOnOff: ToggleButton
    private lateinit var suhuValue: String

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        tvSuhu = findViewById(R.id.nilaiSuhu)
        txtPembatas = findViewById(R.id.inputPembatas)
        btnPembatas = findViewById(R.id.btnPembatas)
        tbOnOff = findViewById(R.id.btnOnOff)

        val databaseRef: DatabaseReference = FirebaseDatabase.getInstance().reference
        databaseRef.addValueEventListener(object : ValueEventListener{
            override fun onDataChange(snapshot: DataSnapshot) {
                suhuValue = snapshot.child("/kipas_mp4/suhu_value").value.toString()
                tvSuhu.text = suhuValue
            }

            override fun onCancelled(error: DatabaseError) {

            }
        })

        btnPembatas.setOnClickListener{
            val pembatasRef: DatabaseReference = FirebaseDatabase.getInstance().getReference("/kipas_mp4/suhu_limit")
            val pembatas: String = txtPembatas.text.toString()
            var nilai = 0
            if(pembatas.isNotEmpty()){
                nilai = Integer.parseInt(pembatas)
            }
            pembatasRef.setValue(nilai)
        }

        tbOnOff.setOnCheckedChangeListener { _, isChecked ->
            val onOff: DatabaseReference = FirebaseDatabase.getInstance().getReference("/kipas_mp4/on_off")
            if (isChecked) onOff.setValue(true)
            else onOff.setValue(false)
        }
    }
}