﻿#pragma checksum "E:\PubnubSilverlight\PubnubSilver\UnsubscribeExample.xaml" "{406ea660-64cf-4c82-b6f0-42d48172a799}" "45771E1BF3CC64DADDC503960A948243"
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
    
    
    public partial class UnsubscribeExample : System.Windows.Controls.Page {
        
        internal System.Windows.Controls.Grid LayoutRoot;
        
        internal System.Windows.Controls.Button Subscribe;
        
        internal System.Windows.Controls.TextBlock lblSubscribe;
        
        internal System.Windows.Controls.TextBlock subMessage;
        
        internal System.Windows.Controls.ListBox lSubscribe;
        
        internal System.Windows.Controls.Button Publish;
        
        internal System.Windows.Controls.TextBlock publishedData;
        
        internal System.Windows.Controls.TextBlock lblPublish;
        
        internal System.Windows.Controls.Button Unsubscribe;
        
        internal System.Windows.Controls.TextBlock lblDisConnect;
        
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
            System.Windows.Application.LoadComponent(this, new System.Uri("/PubnubSilver;component/UnsubscribeExample.xaml", System.UriKind.Relative));
            this.LayoutRoot = ((System.Windows.Controls.Grid)(this.FindName("LayoutRoot")));
            this.Subscribe = ((System.Windows.Controls.Button)(this.FindName("Subscribe")));
            this.lblSubscribe = ((System.Windows.Controls.TextBlock)(this.FindName("lblSubscribe")));
            this.subMessage = ((System.Windows.Controls.TextBlock)(this.FindName("subMessage")));
            this.lSubscribe = ((System.Windows.Controls.ListBox)(this.FindName("lSubscribe")));
            this.Publish = ((System.Windows.Controls.Button)(this.FindName("Publish")));
            this.publishedData = ((System.Windows.Controls.TextBlock)(this.FindName("publishedData")));
            this.lblPublish = ((System.Windows.Controls.TextBlock)(this.FindName("lblPublish")));
            this.Unsubscribe = ((System.Windows.Controls.Button)(this.FindName("Unsubscribe")));
            this.lblDisConnect = ((System.Windows.Controls.TextBlock)(this.FindName("lblDisConnect")));
        }
    }
}

