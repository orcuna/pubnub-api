﻿#pragma checksum "E:\PubnubSilverlight\PubnubSilver\PubnubTest.xaml" "{406ea660-64cf-4c82-b6f0-42d48172a799}" "6F4FEC9E4B1BBCAA4C1DC7E8539B2A32"
//------------------------------------------------------------------------------
// <auto-generated>
//     This code was generated by a tool.
//     Runtime Version:4.0.30319.269
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
// </auto-generated>
//------------------------------------------------------------------------------

using System;
using System.Windows;
using System.Windows.Automation;
using System.Windows.Automation.Peers;
using System.Windows.Automation.Provider;
using System.Windows.Controls;
using System.Windows.Controls.Primitives;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Ink;
using System.Windows.Input;
using System.Windows.Interop;
using System.Windows.Markup;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Media.Imaging;
using System.Windows.Resources;
using System.Windows.Shapes;
using System.Windows.Threading;


namespace PubnubSilver {
    
    
    public partial class PubnubTest : System.Windows.Controls.Page {
        
        internal System.Windows.Controls.Grid LayoutRoot;
        
        internal System.Windows.Controls.Button Subscribe;
        
        internal System.Windows.Controls.TextBlock lblSubscribe;
        
        internal System.Windows.Controls.TextBlock subMessage;
        
        internal System.Windows.Controls.ListBox lSubscribe;
        
        internal System.Windows.Controls.Button Publish;
        
        internal System.Windows.Controls.TextBlock publishedData;
        
        internal System.Windows.Controls.TextBlock lblPublish;
        
        internal System.Windows.Controls.Button History;
        
        internal System.Windows.Controls.TextBlock histMessage;
        
        internal System.Windows.Controls.ListBox lHistory;
        
        internal System.Windows.Controls.Button Time;
        
        internal System.Windows.Controls.TextBlock lblTime;
        
        internal System.Windows.Controls.Button btnUUID;
        
        internal System.Windows.Controls.TextBlock lblUUID;
        
        private bool _contentLoaded;
        
        /// <summary>
        /// InitializeComponent
        /// </summary>
        [System.Diagnostics.DebuggerNonUserCodeAttribute()]
        public void InitializeComponent() {
            if (_contentLoaded) {
                return;
            }
            _contentLoaded = true;
            System.Windows.Application.LoadComponent(this, new System.Uri("/PubnubSilver;component/PubnubTest.xaml", System.UriKind.Relative));
            this.LayoutRoot = ((System.Windows.Controls.Grid)(this.FindName("LayoutRoot")));
            this.Subscribe = ((System.Windows.Controls.Button)(this.FindName("Subscribe")));
            this.lblSubscribe = ((System.Windows.Controls.TextBlock)(this.FindName("lblSubscribe")));
            this.subMessage = ((System.Windows.Controls.TextBlock)(this.FindName("subMessage")));
            this.lSubscribe = ((System.Windows.Controls.ListBox)(this.FindName("lSubscribe")));
            this.Publish = ((System.Windows.Controls.Button)(this.FindName("Publish")));
            this.publishedData = ((System.Windows.Controls.TextBlock)(this.FindName("publishedData")));
            this.lblPublish = ((System.Windows.Controls.TextBlock)(this.FindName("lblPublish")));
            this.History = ((System.Windows.Controls.Button)(this.FindName("History")));
            this.histMessage = ((System.Windows.Controls.TextBlock)(this.FindName("histMessage")));
            this.lHistory = ((System.Windows.Controls.ListBox)(this.FindName("lHistory")));
            this.Time = ((System.Windows.Controls.Button)(this.FindName("Time")));
            this.lblTime = ((System.Windows.Controls.TextBlock)(this.FindName("lblTime")));
            this.btnUUID = ((System.Windows.Controls.Button)(this.FindName("btnUUID")));
            this.lblUUID = ((System.Windows.Controls.TextBlock)(this.FindName("lblUUID")));
        }
    }
}

